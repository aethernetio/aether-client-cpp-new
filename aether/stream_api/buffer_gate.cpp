/*
 * Copyright 2024 Aethernet Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "aether/stream_api/buffer_gate.h"

#include "aether/tele/tele.h"

namespace ae {

BufferGate::BufferedWriteAction::BufferedWriteAction(
    ActionContext action_context, DataBuffer data, TimePoint current_time)
    : StreamWriteAction(action_context),
      data_{std::move(data)},
      data_size_{data_.size()},
      current_time_{current_time} {
  state_ = State::kQueued;
}

TimePoint BufferGate::BufferedWriteAction::Update(TimePoint current_time) {
  if (state_.changed()) {
    switch (state_.Acquire()) {
      case State::kStopped:
        Action::Stop(*this);
        break;
      default:
        break;
    }
  }

  return current_time;
}

void BufferGate::BufferedWriteAction::Stop() {
  if (write_action_) {
    write_action_->Stop();
  } else {
    state_ = State::kStopped;
  }
  Action::Trigger();
}

void BufferGate::BufferedWriteAction::Send(ByteIGate& out_gate) {
  is_sent_ = true;
  // TODO: is it correct to use current_time_ here?
  write_action_ = out_gate.WriteIn(std::move(data_), current_time_);
  state_changed_subscription_ =
      write_action_->state().changed_event().Subscribe(
          [this](auto state) { state_ = state; });

  write_action_subscription_.Push(
      write_action_->SubscribeOnResult([this](auto const&) {
        state_.Acquire();
        Action::Result(*this);
      }),
      write_action_->SubscribeOnError([this](auto const&) {
        state_.Acquire();
        Action::Error(*this);
      }),
      write_action_->SubscribeOnStop([this](auto const&) {
        state_.Acquire();
        Action::Stop(*this);
      }));
}

bool BufferGate::BufferedWriteAction::is_sent() const { return is_sent_; }

std::size_t BufferGate::BufferedWriteAction::size() const { return data_size_; }

BufferGate::BufferOverflowWriteAction::BufferOverflowWriteAction(
    ActionContext action_context)
    : StreamWriteAction(action_context) {
  state_ = State::kFailed;
}

TimePoint BufferGate::BufferOverflowWriteAction::Update(
    TimePoint current_time) {
  if (state_.changed()) {
    switch (state_.Acquire()) {
      case State::kFailed:
        Action::Error(*this);
        break;
      default:
        break;
    }
  }
  return current_time;
}

void BufferGate::BufferOverflowWriteAction::Stop() {}

BufferGate::BufferGate(ActionContext action_context,
                       std::size_t buffer_max_size)
    : action_context_{action_context},
      buffer_max_size_{buffer_max_size},
      buffer_current_size_{0} {
  gate_update_subscription_ =
      EventSubscriber{gate_update_event_}.Subscribe([this]() {
        if ((out_ != nullptr) && out_->is_linked()) {
          DrainBuffer(*out_);
        }
      });
}

ActionView<StreamWriteAction> BufferGate::WriteIn(DataBuffer data,
                                                  TimePoint current_time) {
  auto add_to_buffer = is_write_buffered();
  // add to buffer either if is write buffered or buffer is not empty to observe
  // write order
  if (add_to_buffer || !write_in_buffer_.empty()) {
    if (buffer_free_size() < data.size()) {
      AE_TELED_ERROR("Buffer overflow");
      // decline write
      // TODO: return value
      return BufferOverflowWriteAction{action_context_};
    }

    AE_TELED_DEBUG("Make a buffered write");

    buffer_current_size_ += data.size();

    auto action_it =
        write_in_buffer_.emplace(std::end(write_in_buffer_), action_context_,
                                 std::move(data), current_time);
    auto remove_action = [this, action_it]() {
      buffer_current_size_ -= action_it->size();
      write_in_buffer_.erase(action_it);
      gate_update_event_.Emit();
    };
    write_in_subscription_.Push(
        action_it->FinishedEvent().Subscribe(remove_action));
    return *action_it;
  }

  AE_TELED_DEBUG("Make a direct write");
  return out_->WriteIn(std::move(data), current_time);
}

bool BufferGate::is_write_buffered() const {
  return (out_ == nullptr) || !out_->is_linked() || out_->is_write_buffered();
}

std::size_t BufferGate::buffer_free_size() const {
  return buffer_max_size_ - buffer_current_size_;
}

bool BufferGate::is_linked() const { return out_ && out_->is_linked(); }

void BufferGate::DrainBuffer(OutGate& out) {
  for (auto& action : write_in_buffer_) {
    if (out_->is_write_buffered()) {
      break;
    }
    if (action.is_sent()) {
      continue;
    }
    AE_TELED_DEBUG("Buffer drain");
    action.Send(out);
  }
}

}  // namespace ae
