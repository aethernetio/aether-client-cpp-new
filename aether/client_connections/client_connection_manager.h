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

#ifndef AETHER_CLIENT_CONNECTIONS_CLIENT_CONNECTION_MANAGER_H_
#define AETHER_CLIENT_CONNECTIONS_CLIENT_CONNECTION_MANAGER_H_

#include <map>
#include <vector>

#include "aether/obj/obj.h"
#include "aether/obj/ptr.h"
#include "aether/actions/action_view.h"
#include "aether/actions/action_list.h"

#include "aether/cloud.h"

#include "aether/ae_actions/get_client_cloud_connection.h"
#include "aether/client_connections/client_connection.h"
#include "aether/client_connections/client_server_connection_selector.h"

namespace ae {
class Aether;
class Client;

class ClientConnectionManager : public Obj {
  AE_OBJECT(ClientConnectionManager, Obj, 0)

  friend class CachedClientServerConnectionFactory;

  struct CloudCache {
    template <typename T>
    void Serializator(T& s) {
      s & cloud;
    }

    Cloud::ptr cloud;
    Ptr<ClientServerConnectionSelector> client_stream_selector;
  };

  struct ServerCache {
    template <typename T>
    void Serializator(T& s) {
      s & server;
    }

    Server::ptr server;
    PtrView<ClientServerConnection> client_server_connection;
  };

 public:
  explicit ClientConnectionManager(ObjPtr<Aether> aether, ObjPtr<Client> client,
                                   Domain* domain);

  Ptr<ClientConnection> GetClientConnection();
  ActionView<GetClientCloudConnection> GetClientConnection(Uid client_uid);

  void RegisterCloud(Uid uid,
                     std::vector<ServerDescriptor> const& server_descriptors);

  Ptr<ClientServerConnectionSelector> GetCloudServerConnectionSelector(Uid uid);

  template <typename Dnv>
  void Visit(Dnv& dnv) {
    dnv(*base_ptr_);
    dnv(aether_, client_, client_clouds_, client_servers_);
  }

 private:
  void RegisterCloud(Uid uid, Cloud::ptr cloud);
  void CacheClientServerConnection(
      ServerId server_id, Ptr<ClientServerConnection> const& connection);
  Ptr<ClientServerConnection> FindCacheClientServerConnection(
      ServerId server_id);

  Obj::ptr aether_;
  Obj::ptr client_;

  std::map<Uid, CloudCache> client_clouds_;
  std::map<ServerId, ServerCache> client_servers_;

  Ptr<ActionList<GetClientCloudConnection>> get_client_cloud_connections_;
};
}  // namespace ae

#endif  // AETHER_CLIENT_CONNECTIONS_CLIENT_CONNECTION_MANAGER_H_
