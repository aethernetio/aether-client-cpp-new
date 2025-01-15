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

#ifndef AETHER_TRANSPORT_SERVER_SERVER_SEND_QUEUE_MANAGER_H_
#define AETHER_TRANSPORT_SERVER_SERVER_SEND_QUEUE_MANAGER_H_

#include <queue>

#include "aether/common.h"
#include "aether/state_machine.h"
#include "aether/actions/action.h"
#include "aether/actions/action_view.h"
#include "aether/actions/action_list.h"
#include "aether/actions/action_context.h"
#include "aether/events/multi_subscription.h"

#include "aether/transport/server/server_channel_transport.h"
#include "aether/transport/server/server_send_packet_action.h"

namespace ae {

class ServerSendQueueManagerAction
    : public Action<ServerSendQueueManagerAction> {
  enum class State : std::uint8_t {
    kTransportNotSet,
    kQueueReady,
    kQueueBusy,
    kQueueEmpty,
  };

 public:
  explicit ServerSendQueueManagerAction(ActionContext action_context);

  TimePoint Update(TimePoint current_time) override;

  ActionView<ServerSendPacketAction> Add(ServerSendPacketAction&& send_action);

  void SetTransport(Ptr<ServerChannelTransport> enqueue_transport);

  void ResetTransport();

 private:
  void Enqueue();

  void EnqueueNext();

  StateMachine<State> state_;
  ActionStore<ServerSendPacketAction> actions_;
  std::queue<ActionView<ServerSendPacketAction>> send_queue_;
  Ptr<ServerChannelTransport> enqueue_transport_;

  MultiSubscription send_success_subscriptions_;
  MultiSubscription send_failed_subscriptions_;
  MultiSubscription send_stop_subscriptions_;
  Subscription state_changed_subscription_;
};
}  // namespace ae

#endif  // AETHER_TRANSPORT_SERVER_SERVER_SEND_QUEUE_MANAGER_H_
