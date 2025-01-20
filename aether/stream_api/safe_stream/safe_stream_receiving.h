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

#ifndef AETHER_STREAM_API_SAFE_STREAM_SAFE_STREAM_RECEIVING_H_
#define AETHER_STREAM_API_SAFE_STREAM_SAFE_STREAM_RECEIVING_H_

#include <cstdint>
#include <vector>
#include <deque>

#include "aether/events/events.h"
#include "aether/actions/action.h"
#include "aether/actions/action_context.h"
#include "aether/api_protocol/protocol_context.h"

#include "aether/transport/data_buffer.h"
#include "aether/stream_api/safe_stream/safe_stream_api.h"
#include "aether/stream_api/safe_stream/safe_stream_types.h"
#include "aether/stream_api/safe_stream/safe_stream_config.h"

namespace ae {

struct ReceivingChunk {
  SafeStreamRingIndex offset;
  DataBuffer data;
};

struct ExpectedChunk {
  SafeStreamRingIndex offset;
  std::uint16_t repeat_count;
};

class SafeStreamReceivingAction : public Action<SafeStreamReceivingAction> {
 public:
  using ReceiveEvent = Event<void(DataBuffer&& data)>;
  using SenDataEvent = Event<void(DataBuffer&& data, TimePoint current_time)>;

  SafeStreamReceivingAction(ActionContext action_context,
                            ProtocolContext& protocol_context,
                            SafeStreamConfig const& config);

  TimePoint Update(TimePoint current_time) override;

  ReceiveEvent::Subscriber receive_event();
  SenDataEvent::Subscriber send_data_event();

  void ReceiveSend(SafeStreamRingIndex offset, DataBuffer data);
  void ReceiveRepeat(SafeStreamRingIndex offset, std::uint16_t repeat,
                     DataBuffer data);

 private:
  void AddDataChunk(ReceivingChunk chunk);

  TimePoint CheckChunkChains(TimePoint current_time);
  TimePoint CheckCompletedChains(TimePoint current_time);
  TimePoint CheckMissedOffset(TimePoint current_time);

  void MakeResponse(TimePoint current_time);

  void AddExpectedChunk(SafeStreamRingIndex offset);
  void AddToConfirmationQueue(SafeStreamRingIndex offset);
  void AddToRepeatQueue(SafeStreamRingIndex offset);

  DataBuffer JoinChunks(std::vector<ReceivingChunk>::iterator begin,
                        std::vector<ReceivingChunk>::iterator end);

  ProtocolContext& protocol_context_;
  SafeStreamApi safe_stream_api_;

  std::uint16_t max_window_size_;
  std::uint16_t max_repeat_count_;
  Duration send_confirm_timeout_;
  Duration send_repeat_timeout_;

  TimePoint last_send_confirm_time_;
  TimePoint oldest_repeat_time_;
  SafeStreamRingIndex last_confirmed_offset_;

  ReceiveEvent receive_event_;
  SenDataEvent send_data_event_;

  std::vector<ReceivingChunk> received_data_chunks_;
  std::vector<ExpectedChunk> expected_chunks_;
  std::deque<SafeStreamRingIndex> repeat_queue_;
  std::deque<SafeStreamRingIndex> confirmation_queue_;

  bool repeat_count_exceeded_ = false;
};
}  // namespace ae

#endif  // AETHER_STREAM_API_SAFE_STREAM_SAFE_STREAM_RECEIVING_H_
