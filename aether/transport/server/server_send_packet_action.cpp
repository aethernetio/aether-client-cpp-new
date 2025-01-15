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

#include "aether/transport/server/server_send_packet_action.h"

#include <utility>

#include "aether/transport/server/server_channel_transport.h"

#include "aether/tele/tele.h"

namespace ae {
ServerSendPacketAction::ServerSendPacketAction(ActionContext action_context,
                                               DataBuffer data)
    : PacketSendAction{action_context}, data_{std::move(data)} {
  AE_TELED_DEBUG("Create ServerSendPacketAction");
  state_.Set(State::kQueued);
  state_changed_subscription_ =
      state_.changed_event().Subscribe([this](auto) { this->Trigger(); });
}

ServerSendPacketAction::~ServerSendPacketAction() {
  AE_TELED_DEBUG("Destroy ServerSendPacketAction");
}

ServerSendPacketAction::ServerSendPacketAction(
    ServerSendPacketAction&& other) noexcept
    : PacketSendAction{std::move(static_cast<PacketSendAction&>(other))},
      data_{std::move(other.data_)},
      send_transport_{std::move(other.send_transport_)},
      send_action_{std::move(other.send_action_)} {
  SendSubscriptions(send_action_);
  state_changed_subscription_ =
      state_.changed_event().Subscribe([this](auto) { this->Trigger(); });
}

ServerSendPacketAction& ServerSendPacketAction::operator=(
    ServerSendPacketAction&& other) noexcept {
  PacketSendAction::operator=(std::move(other));
  data_ = std::move(other.data_);
  send_transport_ = std::move(other.send_transport_);
  send_action_ = std::move(other.send_action_);
  SendSubscriptions(send_action_);
  return *this;
}

TimePoint ServerSendPacketAction::Update(TimePoint current_time) {
  if (state_.changed()) {
    switch (state_.Acquire()) {
      case State::kQueued:
      case State::kProgress:
        break;
      case State::kSuccess:
        Action::Result(*this);
        break;
      case State::kStopped:
        Action::Stop(*this);
        break;
      case State::kTimeout:
      case State::kFailed:
      case State::kPanic:
        Action::Error(*this);
        break;
    }
  }
  return current_time;
}

void ServerSendPacketAction::Stop() {
  state_.Set(State::kStopped);
  if (send_action_) {
    send_action_->Stop();
  }
}

void ServerSendPacketAction::Enqueue(
    Ptr<ServerChannelTransport> send_transport) {
  send_transport_ = std::move(send_transport);
  state_.Set(State::kProgress);
  Send(Now());
}

void ServerSendPacketAction::Dequeue() {
  send_transport_.Reset();
  state_.Set(State::kQueued);
}

void ServerSendPacketAction::Send(TimePoint current_time) {
  send_action_ = send_transport_->Send(data_, current_time);
  SendSubscriptions(send_action_);
  AE_TELED_DEBUG("Server Send Packet");
}

void ServerSendPacketAction::SendSubscriptions(
    ActionView<PacketSendAction>& send_action) {
  if (!send_action) {
    return;
  }

  send_success_subscription_ = send_action->SubscribeOnResult(
      [this](auto const& action) { state_.Set(action.state()); });
  send_failed_subscription_ = send_action->SubscribeOnError(
      [this](auto const& action) { state_.Set(action.state()); });
  send_stoped_subscription_ = send_action->SubscribeOnStop(
      [this](auto const& action) { state_.Set(action.state()); });
}

}  // namespace ae
