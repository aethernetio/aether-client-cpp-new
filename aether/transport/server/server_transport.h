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

#ifndef AETHER_TRANSPORT_SERVER_SERVER_TRANSPORT_H_
#define AETHER_TRANSPORT_SERVER_SERVER_TRANSPORT_H_

#include "aether/obj/ptr.h"
#include "aether/events/events.h"
#include "aether/events/multi_subscription.h"
#include "aether/actions/action_context.h"

#include "aether/server.h"
#include "aether/adapters/adapter.h"

#include "aether/transport/itransport.h"
#include "aether/transport/server/server_channel_transport.h"
#include "aether/transport/server/server_send_packet_action.h"

namespace ae {
class Aether;
class ServerSendQueueManagerAction;

class ServerTransport : public ITransport {
  class NotifyServerReconnectAction
      : public NotifyAction<NotifyServerReconnectAction> {
   public:
    using NotifyAction::NotifyAction;
  };

 public:
  ServerTransport(Ptr<Aether> const& aether, Ptr<Adapter> adapter,
                  Ptr<Server> server);
  ~ServerTransport() override;

  void Connect() override;
  ConnectionInfo const& GetConnectionInfo() const override;
  ConnectionSuccessEvent::Subscriber ConnectionSuccess() override;
  ConnectionErrorEvent::Subscriber ConnectionError() override;

  DataReceiveEvent::Subscriber ReceiveEvent() override;

  ActionView<PacketSendAction> Send(DataBuffer data,
                                    TimePoint current_time) override;

  Server const& server() const;

 private:
  void Reconnect();

  ActionContext action_context_;
  Ptr<Server> server_;

  ConnectionInfo connection_info_;
  ConnectionSuccessEvent connection_success_event_;
  ConnectionErrorEvent connection_error_event_;
  DataReceiveEvent data_receive_event_;

  Ptr<class ServerChannelSelector> channel_selector_;
  Ptr<class ServerChannelTransport> channel_transport_;

  Ptr<ServerSendQueueManagerAction> send_queue_manager_action_;
  NotifyServerReconnectAction notify_server_reconnect_action_;

  MultiSubscription send_failed_subscriptions_;

  Subscription connection_success_subscription_;
  Subscription connection_error_subscription_;
  Subscription receive_event_subscription_;
  Subscription notify_reconnect_subscription_;
};
}  // namespace ae

#endif  // AETHER_TRANSPORT_SERVER_SERVER_TRANSPORT_H_
