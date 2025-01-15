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

#include "aether/client_connections/client_server_connection_selector.h"

#include <utility>

#include "aether/tele/tele.h"

namespace ae {
ClientServerConnectionSelector::ClientServerConnectionSelector(
    Ptr<ServerList> server_list,
    Ptr<IClientServerConnectionFactory> client_server_connection_factory)
    : server_list_{std::move(server_list)},
      server_list_it_{std::begin(*server_list_)},
      client_server_connection_factory_{
          std::move(client_server_connection_factory)} {}

Ptr<ClientServerConnection> ClientServerConnectionSelector::NextServer() {
  if (server_list_it_ == std::end(*server_list_)) {
    AE_TELED_ERROR("Server list is over");
    // TODO: how to handle error
    // Next start connection from the begining
    server_list_it_ = std::begin(*server_list_);
    return {};
  }

  auto& server = *server_list_it_;
  server_list_it_++;

  return client_server_connection_factory_->CreateConnection(server);
}
}  // namespace ae
