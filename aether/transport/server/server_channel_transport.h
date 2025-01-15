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

#ifndef AETHER_TRANSPORT_SERVER_SERVER_CHANNEL_TRANSPORT_H_
#define AETHER_TRANSPORT_SERVER_SERVER_CHANNEL_TRANSPORT_H_

#include <optional>

#include "aether/channel.h"
#include "aether/adapters/adapter.h"
#include "aether/actions/action_list.h"
#include "aether/actions/action_context.h"

#include "aether/events/events.h"
#include "aether/events/multi_subscription.h"

#include "aether/transport/itransport.h"
#include "aether/transport/data_buffer.h"
#include "aether/transport/actions/channel_connection_action.h"

namespace ae {
class Aether;

class ServerChannelTransport : public ITransport {
  // action for send data to the low level channel transport
  class ChannelPacketSendAction : public PacketSendAction {
   public:
    explicit ChannelPacketSendAction(
        ActionContext action_context,
        ActionView<PacketSendAction> low_level_send_action,
        TimePoint send_timeout);

    ~ChannelPacketSendAction() override;

    TimePoint Update(TimePoint current_time) override;

    void Stop() override;

   private:
    // start sending data
    void SendData(TimePoint current_time);
    TimePoint CheckTimeout(TimePoint current_time);

    ActionView<PacketSendAction> low_level_send_action_;
    TimePoint send_timeout_;

    MultiSubscription subscriptions_;
    Subscription state_changed_subscription_;
  };

  static Ptr<ChannelConnectionAction> ConnectionFactory(
      ActionContext action_context, Ptr<Aether> aether, Adapter& adapter,
      Channel& channel);

 public:
  ServerChannelTransport(Ptr<Aether> const& aether, Adapter::ptr const& adapter,
                         Channel::ptr const& channel);
  ~ServerChannelTransport() override;

  void Connect() override;

  ConnectionInfo const& GetConnectionInfo() const override;

  ConnectionSuccessEvent::Subscriber ConnectionSuccess() override;
  ConnectionErrorEvent::Subscriber ConnectionError() override;

  DataReceiveEvent::Subscriber ReceiveEvent() override;

  ActionView<PacketSendAction> Send(DataBuffer data,
                                    TimePoint current_time) override;

 private:
  void OnConnection(ChannelConnectionAction const& connection);
  void OnConnectionError(ChannelConnectionAction const& connection);
  void OnTransportConnectionError();

  void OnDataReceive(DataBuffer const& data, TimePoint current_time);

  PtrView<Aether> aether_;
  PtrView<Adapter> adapter_;
  PtrView<Channel> channel_;
  ActionContext action_context_;

  ConnectionInfo connection_info_;
  DataReceiveEvent data_receive_event_;

  ConnectionSuccessEvent connection_success_event_;
  ConnectionErrorEvent connection_error_event_;

  Ptr<ChannelConnectionAction> connection_action_;
  MultiSubscription connection_subscriptions_;

  Ptr<ITransport> low_level_transport_;
  MultiSubscription low_level_transport_subscriptions_;

  ActionList<ChannelPacketSendAction> send_actions_;
};
}  // namespace ae

#endif  // AETHER_TRANSPORT_SERVER_SERVER_CHANNEL_TRANSPORT_H_
