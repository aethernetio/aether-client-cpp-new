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

#include "aether/server_list/server_list.h"

#include <utility>
#include <algorithm>

#include "aether/cloud.h"

namespace ae {
ServerList::ServerInfo::ServerInfo(Server::ptr server)
    : server_{std::move(server)} {}

Server const& ServerList::ServerInfo::server() const { return *server_; }

Server::ptr& ServerList::ServerInfo::get_server() { return server_; }

ServerList::ServerTransportListIterator::ServerTransportListIterator()
    : item_{} {}
ServerList::ServerTransportListIterator::ServerTransportListIterator(
    container_type::iterator it)
    : item_{std::move(it)} {}

ServerList::ServerTransportListIterator
ServerList::ServerTransportListIterator::operator++(int) {
  auto temp = *this;
  ++item_;
  return temp;
}

ServerList::ServerTransportListIterator&
ServerList::ServerTransportListIterator::operator++() {
  ++item_;
  return *this;
}

bool ServerList::ServerTransportListIterator::operator==(
    ServerTransportListIterator const& other) const {
  if (this == &other) {
    return true;
  }
  return item_ == other.item_;
}

bool ServerList::ServerTransportListIterator::operator!=(
    ServerTransportListIterator const& other) const {
  return !(*this == other);
}

ServerList::value_type& ServerList::ServerTransportListIterator::operator*() {
  return *item_;
}

ServerList::value_type const&
ServerList::ServerTransportListIterator::operator*() const {
  return *item_;
}

ServerList::value_type& ServerList::ServerTransportListIterator::operator->() {
  return *item_;
}

ServerList::value_type const&
ServerList::ServerTransportListIterator::operator->() const {
  return *item_;
}

ServerList::ServerList(Ptr<ServeListPolicy> policy, Ptr<Cloud> cloud)
    : policy_{std::move(policy)}, cloud_{std::move(cloud)} {
  assert(cloud_);
  assert(!cloud_->servers().empty());

  BuildList();
}

ServerList::iterator ServerList::begin() {
  return iterator{std::begin(server_transport_list_)};
}

ServerList::iterator ServerList::end() {
  return iterator{std::end(server_transport_list_)};
}

std::size_t ServerList::size() const { return server_transport_list_.size(); }

void ServerList::BuildList() {
  auto& servers = cloud_->servers();

  for (auto& s : servers) {
    if (!s) {
      cloud_->LoadServer(s);
    }
    server_transport_list_.emplace_back(s);
  }

  // apply list policy
  server_transport_list_.erase(
      std::remove_if(std::begin(server_transport_list_),
                     std::end(server_transport_list_),
                     [&](auto const& item) { return policy_->Filter(*item); }),
      std::end(server_transport_list_));

  std::sort(std::begin(server_transport_list_),
            std::end(server_transport_list_),
            [&](auto const& left, auto const& right) {
              return policy_->Preferred(*left, *right);
            });
}
}  // namespace ae
