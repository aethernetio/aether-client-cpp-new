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

#include "aether/client_connections/client_connection_manager.h"

#include <utility>

#include "aether/aether.h"
#include "aether/client.h"

#include "aether/server_list/server_list.h"
#include "aether/server_list/no_filter_server_list_policy.h"

#include "aether/transport/server/server_transport_factory.h"
#include "aether/client_connections/client_cloud_connection.h"
#include "aether/client_connections/iclient_server_connection_factory.h"

#include "aether/tele/tele.h"

namespace ae {

class CachedClientServerConnectionFactory
    : public IClientServerConnectionFactory {
 public:
  CachedClientServerConnectionFactory(
      Ptr<ClientConnectionManager> const& client_connection_manager,
      ActionContext action_context, Ptr<Client> client,
      Ptr<IServerTransportFactory> server_transport_factory)
      : client_connection_manager_{client_connection_manager},
        action_context_{action_context},
        client_{std::move(client)},
        server_transport_factory_{std::move(server_transport_factory)} {}

  Ptr<ClientServerConnection> CreateConnection(
      Ptr<Server> const& server) override {
    auto ccm = client_connection_manager_.Lock();
    assert(ccm);
    auto cached_connection =
        ccm->FindCacheClientServerConnection(server->server_id);
    if (cached_connection) {
      return cached_connection;
    }

    auto connection =
        MakePtr<ClientServerConnection>(MakePtr<ClientToServerStream>(
            action_context_, client_, server->server_id,
            server_transport_factory_->CreateTransport(server)));

    ccm->CacheClientServerConnection(server->server_id, connection);
    return connection;
  }

 private:
  PtrView<ClientConnectionManager> client_connection_manager_;
  ActionContext action_context_;
  Ptr<Client> client_;
  Ptr<IServerTransportFactory> server_transport_factory_;
};

ClientConnectionManager::ClientConnectionManager(ObjPtr<Aether> aether,
                                                 Client::ptr client,
                                                 Domain* domain)
    : Obj(domain), aether_{std::move(aether)}, client_{std::move(client)} {
  client_.SetFlags(ObjFlags{});
  auto client_ptr = Ptr<Client>{client_};
  RegisterCloud(client_ptr->uid(), client_ptr->cloud());
}

Ptr<ClientConnection> ClientConnectionManager::GetClientConnection() {
  auto aether = Ptr<Aether>{aether_};
  auto client_ptr = Ptr<Client>{client_};
  auto action_context = ActionContext{*aether->action_processor};

  AE_TELED_DEBUG("GetClientConnection to self client {}", client_ptr->uid());

  return MakePtr<ClientCloudConnection>(
      action_context, GetCloudServerConnectionSelector(client_ptr->uid()));
}

ActionView<GetClientCloudConnection>
ClientConnectionManager::GetClientConnection(Uid client_uid) {
  AE_TELED_DEBUG("GetClientCloudConnection to another client {}", client_uid);

  auto aether = Ptr<Aether>{aether_};
  auto action_context = ActionContext{*aether->action_processor};
  auto client_ptr = Ptr<Client>{client_};

  if (!get_client_cloud_connections_) {
    get_client_cloud_connections_ =
        MakePtr<ActionList<GetClientCloudConnection>>(action_context);
  }

  auto self_ptr = SelfObjPtr(this);
  assert(self_ptr);

  auto client_server_connection_selector =
      GetCloudServerConnectionSelector(client_ptr->uid());

  auto action = get_client_cloud_connections_->Emplace(
      self_ptr, client_uid, std::move(client_server_connection_selector));

  return action;
}

void ClientConnectionManager::RegisterCloud(
    Uid uid, std::vector<ServerDescriptor> const& server_descriptors) {
  auto aether = Ptr<Aether>{aether_};
  auto client_ptr = Ptr<Client>{client_};

  auto new_cloud = aether->domain_->LoadCopy(aether->cloud_prefab);
  assert(new_cloud);

  for (auto const& descriptor : server_descriptors) {
    auto server_id = descriptor.server_id;
    auto s_cache_it = client_servers_.find(server_id);
    if (s_cache_it != std::end(client_servers_)) {
      new_cloud->AddServer(s_cache_it->second.server);
      continue;
    }

    auto server = aether->domain_->CreateObj<Server>();
    server->server_id = server_id;
    for (auto const& endpoint : descriptor.ips) {
      for (auto const& protocol_port : endpoint.protocol_and_ports) {
        auto channel = server->domain_->CreateObj<Channel>();
        channel->address = IpAddressPortProtocol{
            {endpoint.ip, protocol_port.port}, protocol_port.protocol};
        server->AddChannel(std::move(channel));
      }
    }
    client_servers_[server_id].server = server;
    new_cloud->AddServer(server);
    aether->AddServer(std::move(server));
  }

  // TODO: add adapter logic
  auto& client_cloud = client_clouds_[client_ptr->uid()];
  new_cloud->set_adapter(client_cloud.cloud->adapter());

  client_clouds_[uid].cloud = std::move(new_cloud);
}

void ClientConnectionManager::RegisterCloud(Uid uid, Cloud::ptr cloud) {
  for (auto const& s : cloud->servers()) {
    auto& s_cache = client_servers_[s->server_id];
    if (s_cache.server) {
      continue;
    }
    s_cache.server = s;
  }

  client_clouds_[uid].cloud = std::move(cloud);
}

Ptr<ClientServerConnectionSelector>
ClientConnectionManager::GetCloudServerConnectionSelector(Uid uid) {
  auto cloud_cache = client_clouds_.find(uid);
  if (cloud_cache == client_clouds_.end()) {
    return {};
  }

  if (cloud_cache->second.client_stream_selector) {
    return cloud_cache->second.client_stream_selector;
  }

  auto aether = Ptr<Aether>{aether_};
  auto client_ptr = Ptr<Client>{client_};
  auto self_ptr = SelfObjPtr(this);
  assert(self_ptr);

  if (!cloud_cache->second.cloud) {
    domain_->LoadRoot(cloud_cache->second.cloud);
  }

  auto server_list = MakePtr<ServerList>(MakePtr<NoFilterServerListPolicy>(),
                                         cloud_cache->second.cloud);

  // TODO: add select adapter logic
  auto server_transport_factory = MakePtr<ServerTransportFactory>(
      Ptr<Aether>{aether_}, cloud_cache->second.cloud->adapter());

  auto client_to_server_stream_factory =
      MakePtr<CachedClientServerConnectionFactory>(
          self_ptr, ActionContext{*aether->action_processor}, client_ptr,
          std::move(server_transport_factory));

  cloud_cache->second.client_stream_selector =
      MakePtr<ClientServerConnectionSelector>(
          std::move(server_list), std::move(client_to_server_stream_factory));

  return cloud_cache->second.client_stream_selector;
}

void ClientConnectionManager::CacheClientServerConnection(
    ServerId server_id, Ptr<ClientServerConnection> const& connection) {
  auto it = client_servers_.find(server_id);

  if (it != std::end(client_servers_)) {
    it->second.client_server_connection = connection;
  } else {
    AE_TELED_DEBUG("CacheClientServerStream: server_id {} not found",
                   server_id);
    assert(false);
  }
}

Ptr<ClientServerConnection>
ClientConnectionManager::FindCacheClientServerConnection(ServerId server_id) {
  auto it = client_servers_.find(server_id);
  if (it != std::end(client_servers_)) {
    if (auto s = it->second.client_server_connection.Lock()) {
      return s;
    }
  }
  return {};
}
}  // namespace ae
