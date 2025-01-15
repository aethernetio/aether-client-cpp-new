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

#include "aether/transport/server/server_send_queue_manager.h"

#include <utility>

#include "aether/tele/tele.h"

namespace ae {
ServerSendQueueManagerAction::ServerSendQueueManagerAction(
    ActionContext action_context)
    : Action{action_context} {
  state_.Set(State::kTransportNotSet);
  state_changed_subscription_ =
      state_.changed_event().Subscribe([this](auto) { this->Trigger(); });
}

TimePoint ServerSendQueueManagerAction::Update(TimePoint current_time) {
  if (state_.changed()) {
    switch (state_.Acquire()) {
      case State::kTransportNotSet:
        break;
      case State::kQueueReady:
        Enqueue();
        break;
      case State::kQueueBusy:
      case State::kQueueEmpty:
        break;
    }
  }
  return current_time;
}

ActionView<ServerSendPacketAction> ServerSendQueueManagerAction::Add(
    ServerSendPacketAction&& send_action) {
  auto view = actions_.Add(std::move(send_action));
  send_queue_.push(view);
  if (state_.get() == State::kQueueEmpty) {
    AE_TELED_DEBUG("Enqueue server send action");
    state_.Set(State::kQueueReady);
  }
  return view;
}

void ServerSendQueueManagerAction::SetTransport(
    Ptr<ServerChannelTransport> enqueue_transport) {
  enqueue_transport_ = std::move(enqueue_transport);
  if (state_.get() == State::kTransportNotSet) {
    state_.Set(State::kQueueReady);
  }
}

void ServerSendQueueManagerAction::ResetTransport() {
  enqueue_transport_ = {};
  if (state_.get() == State::kTransportNotSet) {
    state_.Set(State::kTransportNotSet);
  }
}

void ServerSendQueueManagerAction::Enqueue() {
  if (send_queue_.empty()) {
    state_.Set(State::kQueueEmpty);
    return;
  }
  auto& send_action = send_queue_.front();
  send_success_subscriptions_.Push(
      send_action->SubscribeOnResult([this](auto const&) { EnqueueNext(); }));

  send_failed_subscriptions_.Push(send_action->SubscribeOnError(
      [&](auto const&) { send_action->Dequeue(); }));

  send_stop_subscriptions_.Push(
      send_action->SubscribeOnStop([this](auto const&) { EnqueueNext(); }));

  state_.Set(State::kQueueBusy);
  send_action->Enqueue(enqueue_transport_);
}

void ServerSendQueueManagerAction::EnqueueNext() {
  if (!send_queue_.empty()) {
    send_queue_.pop();
  }
  if (state_.get() == State::kQueueBusy) {
    state_.Set(State::kQueueReady);
  }
}

}  // namespace ae
