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

#include "aether/transport/server/server_transport.h"

#include <utility>

#include "aether/aether.h"

#include "aether/transport/server/server_send_queue_manager.h"
#include "aether/transport/server/server_channel_selector.h"
#include "aether/transport/server/server_channel_transport.h"
#include "aether/transport/server/server_send_packet_action.h"

#include "aether/tele/tele.h"

namespace ae {
ServerTransport::ServerTransport(Ptr<Aether> const& aether,
                                 Ptr<Adapter> adapter, Ptr<Server> server)
    : action_context_{*aether->action_processor},
      server_{std::move(server)},
      connection_info_{},
      channel_selector_{
          MakePtr<ServerChannelSelector>(aether, server_, std::move(adapter))},
      send_queue_manager_action_{
          MakePtr<ServerSendQueueManagerAction>(action_context_)} {
  AE_TELED_DEBUG("Server transport for server {} created", server_->server_id);
}

ServerTransport::~ServerTransport() {
  AE_TELED_DEBUG("Destroy ServerTransport");
}

void ServerTransport::Connect() {
  AE_TELED_DEBUG("Server connect");
  notify_reconnect_subscription_.Reset();
  channel_transport_ = channel_selector_->NextChannel();

  if (!channel_transport_) {
    connection_error_event_.Emit();
    return;
  }

  connection_success_subscription_ =
      channel_transport_->ConnectionSuccess().Subscribe([this]() {
        AE_TELED_DEBUG("Server transport connected");
        auto ctci = channel_transport_->GetConnectionInfo();
        connection_info_.destination = ctci.destination;
        connection_info_.max_packet_size = ctci.max_packet_size;
        connection_info_.connection_state = ctci.connection_state;

        send_queue_manager_action_->SetTransport(channel_transport_);

        connection_success_event_.Emit();
      });

  // on connection error try next channel
  connection_error_subscription_ =
      channel_transport_->ConnectionError().Subscribe(
          [this]() { Reconnect(); });

  receive_event_subscription_ = channel_transport_->ReceiveEvent().Subscribe(
      [this](auto const& data, auto current_time) {
        data_receive_event_.Emit(data, current_time);
      });

  channel_transport_->Connect();
  connection_info_.connection_state =
      channel_transport_->GetConnectionInfo().connection_state;
}

ConnectionInfo const& ServerTransport::GetConnectionInfo() const {
  return connection_info_;
}

ServerTransport::ConnectionSuccessEvent::Subscriber
ServerTransport::ConnectionSuccess() {
  return connection_success_event_;
}

ServerTransport::ConnectionErrorEvent::Subscriber
ServerTransport::ConnectionError() {
  return connection_error_event_;
}

ServerTransport::DataReceiveEvent::Subscriber ServerTransport::ReceiveEvent() {
  return data_receive_event_;
}

ActionView<PacketSendAction> ServerTransport::Send(
    DataBuffer data, TimePoint /* current_time */) {
  auto send_action = send_queue_manager_action_->Add(
      ServerSendPacketAction{action_context_, std::move(data)});

  send_failed_subscriptions_.Push(
      send_action->SubscribeOnError([this](auto const& action) {
        AE_TELED_ERROR("Server send action failed with state {}, try reconnect",
                       action.state().get());
        send_queue_manager_action_->ResetTransport();
        // try reconnect
        Reconnect();
      }));

  return send_action;
}

Server const& ServerTransport::server() const { return *server_; }

void ServerTransport::Reconnect() {
  AE_TELED_DEBUG("Server reconnect");
  notify_server_reconnect_action_ =
      NotifyServerReconnectAction{action_context_};

  notify_reconnect_subscription_ =
      notify_server_reconnect_action_
          .SubscribeOnResult([this](auto const&) { Connect(); })
          .Once();

  notify_server_reconnect_action_.Notify();
}

}  // namespace ae
