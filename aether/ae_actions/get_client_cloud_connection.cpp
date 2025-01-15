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

#include "aether/ae_actions/get_client_cloud_connection.h"

#include <utility>

#include "aether/client_connections/client_cloud_connection.h"
#include "aether/client_connections/client_connection_manager.h"

#include "aether/tele/tele.h"

namespace ae {
GetClientCloudConnection::GetClientCloudConnection(
    ActionContext action_context,
    Ptr<ClientConnectionManager> const& client_connection_manager,
    Uid client_uid,
    Ptr<ClientServerConnectionSelector> client_server_connection_selector)
    : Action{action_context},
      action_context_{action_context},
      client_uid_{client_uid},
      client_connection_manager_{client_connection_manager},
      client_server_connection_selector_{
          std::move(client_server_connection_selector)},
      state_{State::kTryCache},
      state_changed_subscription_{state_.changed_event().Subscribe(
          [this](auto) { Action::Trigger(); })} {
  AE_TELED_DEBUG("GetClientCloudConnection()");
}

GetClientCloudConnection::~GetClientCloudConnection() {
  AE_TELED_DEBUG("~GetClientCloudConnection");
}

TimePoint GetClientCloudConnection::Update(TimePoint current_time) {
  AE_TELED_DEBUG("Update()");

  if (state_.changed()) {
    switch (state_.Acquire()) {
      case State::kTryCache:
        TryCache(current_time);
        break;
      case State::kSelectServer:
        SelectServer(current_time);
        break;
      case State::kConnection:
        break;
      case State::kGetCloud:
        GetCloud(current_time);
        break;
      case State::kCreateConnection:
        CreateConnection(current_time);
        break;
      case State::kSuccess:
        Action::Result(*this);
        break;
      case State::kStopped:
        Action::Stop(*this);
        break;
      case State::kFailed:
        Action::Error(*this);
        break;
    }
  }

  return current_time;
}

void GetClientCloudConnection::Stop() {
  if (get_client_cloud_action_) {
    get_client_cloud_action_->Stop();
  }
  state_.Set(State::kStopped);
}

Ptr<ClientConnection> GetClientCloudConnection::client_cloud_connection()
    const {
  return client_cloud_connection_;
}

void GetClientCloudConnection::TryCache(TimePoint /* current_time */) {
  auto ccm = client_connection_manager_.Lock();
  if (!ccm) {
    AE_TELED_ERROR("Client connection manager is null");
    state_.Set(State::kFailed);
  }
  auto cloud_server_selector =
      ccm->GetCloudServerConnectionSelector(client_uid_);
  if (cloud_server_selector) {
    AE_TELED_INFO("Found cached connection");
    client_cloud_connection_ =
        CreateConnection(std::move(cloud_server_selector));
    state_.Set(State::kSuccess);
    return;
  }
  state_.Set(State::kSelectServer);
}

void GetClientCloudConnection::SelectServer(TimePoint /* current_time */) {
  for (auto i = 0; i < 2; ++i) {
    client_server_connection_ =
        client_server_connection_selector_->NextServer();
    if (!client_server_connection_) {
      continue;
    }
  }

  if (!client_server_connection_) {
    AE_TELED_ERROR("Failed to select server");
    state_.Set(State::kFailed);
    return;
  }

  if (client_server_connection_->connection_state() ==
      ConnectionState::kConnected) {
    state_.Set(State::kGetCloud);
    return;
  }

  connection_subscription_ =
      client_server_connection_->connected_event()
          .Subscribe([this]() { state_.Set(State::kGetCloud); })
          .Once();
  disconnection_subscription_ =
      client_server_connection_->connection_error_event()
          .Subscribe([this]() { state_.Set(State::kSelectServer); })
          .Once();
  // wait connection
  state_.Set(State::kConnection);
}

void GetClientCloudConnection::GetCloud(TimePoint /* current_time */) {
  get_client_cloud_action_.emplace(
      action_context_, client_server_connection_->server_stream(), client_uid_);

  get_client_cloud_subscriptions_.Push(
      get_client_cloud_action_->SubscribeOnError([this](auto const&) {
        AE_TELED_DEBUG("GetClientCloudAction failed");
        state_.Set(State::kSelectServer);
      }),
      get_client_cloud_action_->SubscribeOnResult(
          [this](auto const&) { state_.Set(State::kCreateConnection); }),
      get_client_cloud_action_->SubscribeOnStop(
          [this](auto const&) { state_.Set(State::kStopped); }));
}

void GetClientCloudConnection::CreateConnection(TimePoint /* current_time */) {
  assert(get_client_cloud_action_.has_value());

  auto servers = get_client_cloud_action_->server_descriptors();

  auto ccm = client_connection_manager_.Lock();
  ccm->RegisterCloud(client_uid_, servers);

  auto cloud_server_selector =
      ccm->GetCloudServerConnectionSelector(client_uid_);
  assert(cloud_server_selector);

  client_cloud_connection_ = CreateConnection(std::move(cloud_server_selector));
  state_.Set(State::kSuccess);
}

Ptr<ClientConnection> GetClientCloudConnection::CreateConnection(
    Ptr<ClientServerConnectionSelector> client_to_server_stream_selector) {
  return MakePtr<ClientCloudConnection>(
      action_context_, std::move(client_to_server_stream_selector));
}

}  // namespace ae
