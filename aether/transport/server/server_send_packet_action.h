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

#ifndef AETHER_TRANSPORT_SERVER_SERVER_SEND_PACKET_ACTION_H_
#define AETHER_TRANSPORT_SERVER_SERVER_SEND_PACKET_ACTION_H_

#include "aether/common.h"
#include "aether/obj/ptr.h"
#include "aether/actions/action_view.h"
#include "aether/actions/action_context.h"
#include "aether/transport/data_buffer.h"
#include "aether/events/event_subscription.h"
#include "aether/transport/actions/packet_send_action.h"

namespace ae {
class ServerChannelTransport;

class ServerSendPacketAction : public PacketSendAction {
 public:
  ServerSendPacketAction(ActionContext action_context, DataBuffer data);
  ~ServerSendPacketAction() override;

  ServerSendPacketAction(ServerSendPacketAction const& other) = delete;
  ServerSendPacketAction(ServerSendPacketAction&& other) noexcept;

  ServerSendPacketAction& operator=(ServerSendPacketAction const& other) =
      delete;
  ServerSendPacketAction& operator=(ServerSendPacketAction&& other) noexcept;

  TimePoint Update(TimePoint current_time) override;

  void Stop() override;

  void Enqueue(Ptr<ServerChannelTransport> send_transport);
  void Dequeue();

 private:
  void Send(TimePoint current_time);
  void SendSubscriptions(ActionView<PacketSendAction>& send_action);

  DataBuffer data_;

  Ptr<ServerChannelTransport> send_transport_;
  ActionView<PacketSendAction> send_action_;

  Subscription send_success_subscription_;
  Subscription send_failed_subscription_;
  Subscription send_stoped_subscription_;
  Subscription state_changed_subscription_;
};
}  // namespace ae

#endif  // AETHER_TRANSPORT_SERVER_SERVER_SEND_PACKET_ACTION_H_
