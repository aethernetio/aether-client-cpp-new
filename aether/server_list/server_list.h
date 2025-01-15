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

#ifndef AETHER_SERVER_LIST_SERVER_LIST_H_
#define AETHER_SERVER_LIST_SERVER_LIST_H_

#include <vector>

#include "aether/obj/ptr.h"

#include "aether/server.h"
#include "aether/server_list/list_policy.h"

namespace ae {
class Aether;
class Cloud;

class ServerList {
  class ServerInfo {
   public:
    explicit ServerInfo(Server::ptr server);

    Server const& server() const;

    Server::ptr& get_server();

   private:
    Server::ptr server_;
  };

  friend class ServerTransportListIterator;

 public:
  using container_type = std::vector<Server::ptr>;
  using value_type = Server::ptr;

  class ServerTransportListIterator {
   public:
    ServerTransportListIterator();
    explicit ServerTransportListIterator(container_type::iterator it);

    ServerTransportListIterator operator++(int);
    ServerTransportListIterator& operator++();

    bool operator==(ServerTransportListIterator const& other) const;
    bool operator!=(ServerTransportListIterator const& other) const;

    value_type& operator*();
    value_type const& operator*() const;

    value_type& operator->();
    value_type const& operator->() const;

   private:
    container_type::iterator item_;
  };

  using iterator = ServerTransportListIterator;

  ServerList(Ptr<ServeListPolicy> policy, Ptr<Cloud> cloud);

  iterator begin();
  iterator end();
  std::size_t size() const;

 private:
  void BuildList();

  Ptr<ServeListPolicy> policy_;
  Ptr<Cloud> cloud_;

  container_type server_transport_list_;
};
}  // namespace ae

#endif  // AETHER_SERVER_LIST_SERVER_LIST_H_
