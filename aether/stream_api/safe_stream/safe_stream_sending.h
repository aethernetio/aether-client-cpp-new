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

#ifndef AETHER_STREAM_API_SAFE_STREAM_SAFE_STREAM_SENDING_H_
#define AETHER_STREAM_API_SAFE_STREAM_SAFE_STREAM_SENDING_H_

#include <deque>
#include <vector>
#include <list>

#include "aether/common.h"
#include "aether/events/events.h"
#include "aether/actions/action.h"
#include "aether/actions/action_view.h"
#include "aether/actions/action_list.h"
#include "aether/actions/action_context.h"
#include "aether/events/multi_subscription.h"

#include "aether/transport/data_buffer.h"

#include "aether/stream_api/safe_stream/safe_stream_api.h"
#include "aether/stream_api/safe_stream/safe_stream_types.h"
#include "aether/stream_api/safe_stream/sending_data_action.h"

namespace ae {

class SafeStreamSendingAction : public Action<SafeStreamSendingAction> {
 public:
  using SenDataEvent = Event<void(SafeStreamRingIndex offset, DataBuffer data,
                                  TimePoint current_time)>;

  SafeStreamSendingAction(ActionContext action_context,
                          ProtocolContext& protocol_context,
                          SafeStreamConfig const& config);

  ~SafeStreamSendingAction() override;

  TimePoint Update(TimePoint current_time) override;

  SenDataEvent::Subscriber send_data_event();

  ActionView<SendingDataAction> SendData(DataBuffer data);
  void Confirm(SafeStreamRingIndex offset);
  void RequestRepeatSend(SafeStreamRingIndex offset);

  void ReportSendSuccess(SafeStreamRingIndex offset);
  void ReportSendStopped(SafeStreamRingIndex offset);
  void ReportSendError(SafeStreamRingIndex offset);

  void set_max_data_size(std::size_t max_data_size);

 private:
  std::vector<SendingDataIter> SplitData(SendingData const& sending_data) const;
  void ConfirmDataChunks(SafeStreamRingIndex offset);

  void SendData(TimePoint current_time);
  void SendFirst(SendingChunk const& chunk, TimePoint current_time);
  void SendRepeat(SendingChunk const& chunk, TimePoint current_time);
  void SendDataBuffer(SafeStreamRingIndex offset, DataBuffer packet,
                      TimePoint current_time);

  void StopSending(SafeStreamRingIndex offset);
  void SendFailed(SafeStreamRingIndex offset);
  void EraseSendingState(SafeStreamRingIndex offset);

  std::list<ActionView<SendingDataAction>>::iterator FindSendingData(
      SafeStreamRingIndex offset);

  TimePoint HandleTimeouts(TimePoint current_time);
  void RetryRepeatSend(SafeStreamRingIndex offset);

  ProtocolContext& protocol_context_;
  std::size_t max_data_size_;
  std::uint16_t buffer_capacity_;
  std::uint16_t window_size_;
  std::uint16_t max_repeat_count_;
  Duration wait_confirm_timeout_;
  SafeStreamApi safe_stream_api_;

  SenDataEvent send_data_event_;

  ActionList<SendingDataAction> sending_data_actions_;
  std::list<ActionView<SendingDataAction>>
      sending_data_list_;  // main data store list

  std::deque<SendingChunk> sending_chunk_list_;  // unconfirmed data refs list
  std::deque<SendingChunk>
      send_queue_;  // data res to send in the next iterations
  std::deque<OffsetTime>
      offset_send_times_;           // sent offsets times waiting confirmation
  std::uint16_t data_buffer_size_;  // whole data buffer size

  SafeStreamRingIndex last_confirmed_;
  SafeStreamRingIndex next_to_add_;

  MultiSubscription send_data_subscriptions_;
};

}  // namespace ae

#endif  // AETHER_STREAM_API_SAFE_STREAM_SAFE_STREAM_SENDING_H_
