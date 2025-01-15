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

#include "aether/stream_api/safe_stream/safe_stream_sending.h"

#include <iterator>
#include <utility>
#include <algorithm>

#include "aether/api_protocol/packet_builder.h"

#include "aether/stream_api/safe_stream/safe_stream_types.h"

#include "aether/tele/tele.h"

namespace ae {
SafeStreamSendingAction::SafeStreamSendingAction(
    ActionContext action_context, ProtocolContext& protocol_context,
    SafeStreamConfig const& config)
    : Action{action_context},
      protocol_context_{protocol_context},
      max_data_size_{config.max_data_size},
      buffer_capacity_{config.buffer_capacity},
      window_size_{config.window_size},
      max_repeat_count_{config.max_repeat_count},
      wait_confirm_timeout_{config.wait_confirm_timeout},
      sending_data_actions_{action_context},
      data_buffer_size_{},
      last_confirmed_{},
      next_to_add_{} {}

SafeStreamSendingAction::~SafeStreamSendingAction() = default;

TimePoint SafeStreamSendingAction::Update(TimePoint current_time) {
  auto new_time = HandleTimeouts(current_time);

  SendData(current_time);
  return new_time;
}

SafeStreamSendingAction::SenDataEvent::Subscriber
SafeStreamSendingAction::send_data_event() {
  return send_data_event_;
}

ActionView<SendingDataAction> SafeStreamSendingAction::SendData(
    DataBuffer data) {
  auto size = data.size();
  if ((data_buffer_size_ + size) > buffer_capacity_) {
    return {};
  }

  auto offset = next_to_add_;
  auto sending_data = SendingData{offset, std::move(data)};
  next_to_add_ = sending_data.OffsetRange().second + 1;

  auto& sending_action = sending_data_list_.emplace_back(
      sending_data_actions_.Emplace(std::move(sending_data)));

  auto data_iters = SplitData(sending_action->sending_data());
  for (auto& d : std::move(data_iters)) {
    auto& ref = sending_chunk_list_.emplace_back(SendingChunk{{}, d});
    send_queue_.push_back(ref);
  }

  data_buffer_size_ += static_cast<SafeStreamRingIndex::type>(size);

  send_data_subscriptions_.Push(sending_action->stop_event().Subscribe(
      [this, offset]() { StopSending(offset); }));

  Action::Trigger();

  return ActionView{sending_action};
}

void SafeStreamSendingAction::Confirm(SafeStreamRingIndex offset) {
  auto distance = last_confirmed_.Distance(offset);
  if (distance <= window_size_) {
    ConfirmDataChunks(offset);
    last_confirmed_ = offset + 1;
  }
  AE_TELED_DEBUG("Receive confirmed offset {}", offset);
  this->Trigger();
}

void SafeStreamSendingAction::RequestRepeatSend(SafeStreamRingIndex offset) {
  auto it = std::find_if(
      std::begin(sending_chunk_list_), std::end(sending_chunk_list_),
      [&](auto const& sch) { return sch.data_iter.begin_offset() == offset; });

  if (it == std::end(sending_chunk_list_)) {
    AE_TELED_ERROR("Repeat chunk not found");
    return;
  }

  send_queue_.push_front(*it);
  AE_TELED_DEBUG("Repeat chunk offset:{} count:{}",
                 it->data_iter.begin_offset(), it->repeat_count);

  this->Trigger();
}

void SafeStreamSendingAction::ReportSendSuccess(
    SafeStreamRingIndex /* offset */) {}

void SafeStreamSendingAction::ReportSendStopped(SafeStreamRingIndex offset) {
  AE_TELED_DEBUG("StopSending for offset:{}", offset);

  auto it = FindSendingData(offset);
  if (it == std::end(sending_data_list_)) {
    AE_TELED_ERROR("Stopping offset in sending data not found");
    return;
  }

  StopSending((*it)->sending_data().offset);
}

void SafeStreamSendingAction::ReportSendError(SafeStreamRingIndex offset) {
  AE_TELED_ERROR("Send error for offset:{}", offset);

  auto it = FindSendingData(offset);
  if (it == std::end(sending_data_list_)) {
    AE_TELED_ERROR("Failed offset in sending data not found");
    return;
  }

  SendFailed((*it)->sending_data().offset);
}

void SafeStreamSendingAction::set_max_data_size(std::size_t max_data_size) {
  // TODO: add update sending_chunk_list
  max_data_size_ = max_data_size;
}

std::vector<SendingDataIter> SafeStreamSendingAction::SplitData(
    SendingData const& sending_data) const {
  if (sending_data.data.size() <= max_data_size_) {
    return {sending_data.Iter(sending_data.offset, max_data_size_)};
  }

  auto result = std::vector<SendingDataIter>{};
  result.reserve(sending_data.data.size() / max_data_size_ + 1);
  auto offset = sending_data.offset;

  while (sending_data.offset.Distance(offset) < sending_data.data.size()) {
    result.emplace_back(sending_data.Iter(offset, max_data_size_));
    offset = result.back().end_offset();
  }
  return result;
}

void SafeStreamSendingAction::ConfirmDataChunks(SafeStreamRingIndex offset) {
  EraseSendingState(offset);

  // remove main data list
  sending_data_list_.remove_if([&](auto& sda) {
    if (!sda) {
      return true;
    }
    auto const& sending_data = sda->sending_data();
    auto complete =
        (sending_data.offset +
         static_cast<SafeStreamRingIndex::type>(sending_data.data.size()))
            .Distance(offset) < window_size_;
    if (complete) {
      sda->Success();
      data_buffer_size_ -=
          static_cast<SafeStreamRingIndex::type>(sending_data.data.size());
    }

    return complete;
  });
}

void SafeStreamSendingAction::SendData(TimePoint current_time) {
  // send one chunk from send_queue
  if (send_queue_.empty()) {
    return;
  }
  auto const& send_chunk = send_queue_.front();

  if (last_confirmed_.Distance(send_chunk.data_iter.end_offset()) >
      window_size_) {
    AE_TELED_WARNING("Window size exceeded");
    return;
  }

  auto send_offset = send_chunk.data_iter.begin_offset();
  if (send_chunk.repeat_count == 0) {
    SendFirst(send_chunk, current_time);
  } else {
    SendRepeat(send_chunk, current_time);
  }
  offset_send_times_.emplace_back(OffsetTime{current_time, send_offset});
  send_queue_.pop_front();
}

void SafeStreamSendingAction::SendFirst(SendingChunk const& chunk,
                                        TimePoint current_time) {
  AE_TELED_DEBUG("SendFirst chunk offset:{}", chunk.data_iter.begin_offset());

  auto packet = PacketBuilder{
      protocol_context_,
      PackMessage{
          safe_stream_api_,
          SafeStreamApi::Send{
              {},
              static_cast<SafeStreamRingIndex::type>(
                  chunk.data_iter.begin_offset()),
              {chunk.data_iter.begin(), chunk.data_iter.end()},
          },
      },
  };

  SendDataBuffer(chunk.data_iter.begin_offset(), std::move(packet),
                 current_time);
}

void SafeStreamSendingAction::SendRepeat(SendingChunk const& chunk,
                                         TimePoint current_time) {
  AE_TELED_DEBUG("SendRepeat chunk offset:{} count:{}",
                 chunk.data_iter.begin_offset(), chunk.repeat_count);

  auto packet = PacketBuilder{
      protocol_context_,
      PackMessage{
          safe_stream_api_,
          SafeStreamApi::Repeat{
              {},
              chunk.repeat_count,
              static_cast<SafeStreamRingIndex::type>(
                  chunk.data_iter.begin_offset()),
              {chunk.data_iter.begin(), chunk.data_iter.end()},
          },
      },
  };

  SendDataBuffer(chunk.data_iter.begin_offset(), std::move(packet),
                 current_time);
}

void SafeStreamSendingAction::SendDataBuffer(SafeStreamRingIndex offset,
                                             DataBuffer packet,
                                             TimePoint current_time) {
  send_data_event_.Emit(offset, std::move(packet), current_time);
}

void SafeStreamSendingAction::StopSending(SafeStreamRingIndex offset) {
  AE_TELED_DEBUG("StopSending offset:{}", offset);
  auto it = FindSendingData(offset);

  if (it == std::end(sending_data_list_)) {
    AE_TELED_ERROR("Sending action not found");
    assert(false);
    return;
  }
  (*it)->Stopped();

  EraseSendingState((*it)->sending_data().offset +
                    static_cast<SafeStreamRingIndex::type>(
                        (*it)->sending_data().data.size()));

  sending_data_list_.erase(it);
}

void SafeStreamSendingAction::SendFailed(SafeStreamRingIndex offset) {
  AE_TELED_ERROR("Send repeat failed offset {}", offset);
  auto it = FindSendingData(offset);
  if (it == std::end(sending_data_list_)) {
    AE_TELED_ERROR("Sending action not found");
    assert(false);
    return;
  }
  (*it)->Failed();

  EraseSendingState((*it)->sending_data().offset +
                    static_cast<SafeStreamRingIndex::type>(
                        (*it)->sending_data().data.size()));

  sending_data_list_.erase(it);
}

void SafeStreamSendingAction::EraseSendingState(SafeStreamRingIndex offset) {
  // remove from waiting confirmation
  offset_send_times_.erase(std::remove_if(std::begin(offset_send_times_),
                                          std::end(offset_send_times_),
                                          [&](auto const& ost) {
                                            return ost.offset.Distance(offset) <
                                                   window_size_;
                                          }),
                           std::end(offset_send_times_));
  // remove from pending queue
  send_queue_.erase(
      std::remove_if(std::begin(send_queue_), std::end(send_queue_),
                     [&](auto const& sch) {
                       return (
                           (sch.data_iter.end_offset() - 1).Distance(offset) <
                           window_size_);
                     }),
      std::end(send_queue_));

  // remove from unconfirmed list
  sending_chunk_list_.erase(
      remove_if(std::begin(sending_chunk_list_), std::end(sending_chunk_list_),
                [&](auto const& sch) {
                  return ((sch.data_iter.end_offset() - 1).Distance(offset) <
                          window_size_);
                }),
      std::end(sending_chunk_list_));
}

TimePoint SafeStreamSendingAction::HandleTimeouts(TimePoint current_time) {
  if (offset_send_times_.empty()) {
    return current_time;
  }
  auto selected_ost = offset_send_times_.front();
  if ((selected_ost.time + wait_confirm_timeout_) < current_time) {
    offset_send_times_.pop_front();
    // confirmation timeout
    RetryRepeatSend(selected_ost.offset);
    return current_time;
  }
  return selected_ost.time;
}

void SafeStreamSendingAction::RetryRepeatSend(SafeStreamRingIndex offset) {
  auto it = std::find_if(
      std::begin(sending_chunk_list_), std::end(sending_chunk_list_),
      [&](auto const& sch) { return sch.data_iter.begin_offset() == offset; });

  if (it == std::end(sending_chunk_list_)) {
    AE_TELED_ERROR("Repeat chunk not found");
    return;
  }

  it->repeat_count++;
  if (it->repeat_count > max_repeat_count_) {
    AE_TELED_ERROR("Repeat chunk count exceeded");
    SendFailed(it->data_iter.begin_offset());
    return;
  }

  send_queue_.push_front(*it);
  AE_TELED_DEBUG("Repeat chunk offset:{} count:{}",
                 it->data_iter.begin_offset(), it->repeat_count);

  this->Trigger();
}

std::list<ActionView<SendingDataAction>>::iterator
SafeStreamSendingAction::FindSendingData(SafeStreamRingIndex offset) {
  return std::find_if(
      std::begin(sending_data_list_), std::end(sending_data_list_),
      [&](auto& send_action) {
        if (!send_action) {
          return false;
        }
        return send_action->sending_data().offset.Distance(offset) <
               static_cast<SafeStreamRingIndex::type>(
                   send_action->sending_data().data.size());
      });
}
}  // namespace ae
