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

#include "aether/stream_api/safe_stream/sending_data_action.h"

namespace ae {
SendingDataIter::SendingDataIter(SafeStreamRingIndex b_off,
                                 std::size_t max_size,
                                 SendingData const& s_data)
    : begin_offset_{b_off}, sending_data_{&s_data} {
  assert(sending_data_->offset.Distance(begin_offset_) <=
         static_cast<SafeStreamRingIndex::type>(sending_data_->data.size()));

  end_offset_ =
      begin_offset_ +
      static_cast<SafeStreamRingIndex::type>(std::min(
          max_size, sending_data_->data.size() -
                        sending_data_->offset.Distance(begin_offset_)));
}

DataBuffer::const_iterator SendingDataIter::begin() const {
  return sending_data_->data.begin() +
         static_cast<std::ptrdiff_t>(
             sending_data_->offset.Distance(begin_offset_));
}

DataBuffer::const_iterator SendingDataIter::end() const {
  return sending_data_->data.begin() +
         static_cast<std::ptrdiff_t>(
             sending_data_->offset.Distance(end_offset_));
}

SafeStreamRingIndex SendingDataIter::begin_offset() const {
  return begin_offset_;
}

SafeStreamRingIndex SendingDataIter::end_offset() const { return end_offset_; }

std::size_t SendingDataIter::size() const {
  return begin_offset_.Distance(end_offset_);
}

std::pair<SafeStreamRingIndex, SafeStreamRingIndex> SendingData::OffsetRange()
    const {
  return std::make_pair(
      offset, offset + static_cast<SafeStreamRingIndex::type>(data.size() - 1));
}

SendingDataIter SendingData::Iter(SafeStreamRingIndex start_offset,
                                  std::size_t max_size) const {
  return SendingDataIter{start_offset, max_size, *this};
}

SendingDataAction::SendingDataAction(ActionContext action_context,
                                     SendingData sending_data)
    : Action(action_context),
      sending_data_{std::move(sending_data)},
      state_{State::kWaiting} {}

TimePoint SendingDataAction::Update(TimePoint current_time) {
  if (!state_.changed()) {
    return current_time;
  }
  switch (state_.Acquire()) {
    case State::kWaiting:
      break;
    case State::kDone:
      Action::Result(*this);
      break;
    case State::kStopped:
      Action::Stop(*this);
      break;
    case State::kFailed:
      Action::Error(*this);
      break;
  }

  return current_time;
}

SendingData& SendingDataAction::sending_data() { return sending_data_; }

EventSubscriber<void()> SendingDataAction::stop_event() { return stop_event_; }

void SendingDataAction::Stop() { stop_event_.Emit(); }

void SendingDataAction::Success() {
  state_.Set(State::kDone);
  Action::Trigger();
}
void SendingDataAction::Stopped() {
  state_.Set(State::kStopped);
  Action::Trigger();
}
void SendingDataAction::Failed() {
  state_.Set(State::kFailed);
  Action::Trigger();
}
}  // namespace ae
