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

#include "aether/transport/server/server_channel_transport.h"

#include <chrono>
#include <utility>

#include "aether/config.h"

#include "aether/aether.h"
#include "aether/address.h"

#include "aether/transport/actions/ip_channel_connection.h"
#include "aether/transport/actions/name_address_channel_connection.h"

#include "aether/tele/tele.h"

namespace ae {
namespace _internal {
#if AE_SUPPORT_CLOUD_DNS
inline Ptr<ChannelConnectionAction> MakeConnectionAction(
    ActionContext action_context, Ptr<Aether> aether, Adapter& adapter,
    NameAddress const& name_address) {
  return MakePtr<NameAddressChannelConnectionAction>(
      action_context, name_address, aether, adapter);
}
#endif

inline Ptr<ChannelConnectionAction> MakeConnectionAction(
    ActionContext action_context, Ptr<Aether> /* aether */, Adapter& adapter,
    IpAddressPortProtocol const& ip_addr) {
  return MakePtr<IpAddressChannelConnectionAction>(action_context, ip_addr,
                                                   adapter);
}
}  // namespace _internal

ServerChannelTransport::ChannelPacketSendAction::ChannelPacketSendAction(
    ActionContext action_context,
    ActionView<PacketSendAction> low_level_send_action, TimePoint send_timeout)
    : PacketSendAction{action_context},
      low_level_send_action_{std::move(low_level_send_action)},
      send_timeout_{std::move(send_timeout)} {
  state_changed_subscription_ = state_.changed_event().Subscribe(
      [this](auto const&) { Action::Trigger(); });

  subscriptions_.Push(
      low_level_send_action_->state().changed_event().Subscribe(
          [this](auto state) { state_.Set(state); }),
      low_level_send_action_->SubscribeOnResult([this](auto const& action) {
        state_.Set(action.state());
        Action::Result(*this);
      }),
      low_level_send_action_->SubscribeOnError([this](auto const& action) {
        state_.Set(action.state());
        Action::Error(*this);
      }),
      low_level_send_action_->SubscribeOnStop([this](auto const& action) {
        state_.Set(action.state());
        Action::Stop(*this);
      }));
}

ServerChannelTransport::ChannelPacketSendAction::~ChannelPacketSendAction() {
  AE_TELED_DEBUG("Destroy ChannelPacketSendAction");
}

TimePoint ServerChannelTransport::ChannelPacketSendAction::Update(
    TimePoint current_time) {
  if (state_.get() == State::kProgress) {
    return CheckTimeout(current_time);
  }

  return current_time;
}

void ServerChannelTransport::ChannelPacketSendAction::Stop() {
  low_level_send_action_->Stop();
}

TimePoint ServerChannelTransport::ChannelPacketSendAction::CheckTimeout(
    TimePoint current_time) {
  if (send_timeout_ < current_time) {
    return send_timeout_;
  } else {
    low_level_send_action_->Stop();
    state_.Set(State::kTimeout);
    return current_time;
  }
}

Ptr<ChannelConnectionAction> ServerChannelTransport::ConnectionFactory(
    ActionContext action_context, Ptr<Aether> aether, Adapter& adapter,
    Channel& channel) {
  auto address = channel.address;

  return std::visit(
      [&](auto const& addr) {
        return _internal::MakeConnectionAction(action_context, aether, adapter,
                                               addr);
      },
      address);
}

ServerChannelTransport::ServerChannelTransport(Ptr<Aether> const& aether,
                                               Adapter::ptr const& adapter,
                                               Channel::ptr const& channel)
    : aether_{aether},
      adapter_{adapter},
      channel_{channel},
      action_context_{*aether->action_processor},
      connection_info_{},
      send_actions_{action_context_} {
  AE_TELED_DEBUG("Create ServerChannelTransport");
}

ServerChannelTransport::~ServerChannelTransport() {
  AE_TELED_DEBUG("Destroy ServerChannelTransport");
}

void ServerChannelTransport::Connect() {
  connection_info_.connection_state = ConnectionState::kConnecting;
  auto aether = aether_.Lock();
  assert(aether);

  auto adapter = adapter_.Lock();
  assert(adapter);
  auto channel = channel_.Lock();
  assert(channel);

  connection_action_ =
      ConnectionFactory(action_context_, aether, *adapter, *channel);

  connection_subscriptions_.Push(
      connection_action_->SubscribeOnResult(
          [this](auto const& connection) { OnConnection(connection); }),
      connection_action_->SubscribeOnError(
          [this](auto const& connection) { OnConnectionError(connection); }),
      connection_action_->SubscribeOnStop(
          [this](auto const& connection) { OnConnectionError(connection); }),
      connection_action_->FinishedEvent().Subscribe(
          [this]() { connection_action_.Reset(); }));
}

ConnectionInfo const& ServerChannelTransport::GetConnectionInfo() const {
  return connection_info_;
}

ITransport::ConnectionSuccessEvent::Subscriber
ServerChannelTransport::ConnectionSuccess() {
  return connection_success_event_;
}

ITransport::ConnectionErrorEvent::Subscriber
ServerChannelTransport::ConnectionError() {
  return connection_error_event_;
}

ITransport::DataReceiveEvent::Subscriber
ServerChannelTransport::ReceiveEvent() {
  return data_receive_event_;
}

ActionView<PacketSendAction> ServerChannelTransport::Send(
    DataBuffer data, TimePoint current_time) {
  assert(low_level_transport_);
  // TODO: configuration for timeouts
  return send_actions_.Emplace(
      low_level_transport_->Send(std::move(data), current_time),
      current_time + std::chrono::seconds{1});
}

void ServerChannelTransport::OnConnection(
    ChannelConnectionAction const& connection) {
  low_level_transport_ = connection.transport();

  low_level_transport_subscriptions_.Push(
      low_level_transport_->ReceiveEvent().Subscribe(
          [this](auto const& data, auto current_time) {
            OnDataReceive(data, current_time);
          }),
      low_level_transport_->ConnectionError().Subscribe(
          [this]() { OnTransportConnectionError(); }));

  connection_info_ = connection.connection_info();
  connection_info_.connection_state = ConnectionState::kConnected;
  connection_success_event_.Emit();
}

void ServerChannelTransport::OnConnectionError(
    ChannelConnectionAction const& /* connection */) {
  AE_TELED_ERROR("ServerChannelTransport::OnConnectionError");
  low_level_transport_.Reset();

  connection_error_event_.Emit();
}

void ServerChannelTransport::OnTransportConnectionError() {
  // TODO:
}

void ServerChannelTransport::OnDataReceive(DataBuffer const& data,
                                           TimePoint current_time) {
  data_receive_event_.Emit(data, current_time);
}

}  // namespace ae
