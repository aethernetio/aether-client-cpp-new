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

#ifndef AETHER_TRANSPORT_SERVER_SERVER_CHANNEL_SELECTOR_H_
#define AETHER_TRANSPORT_SERVER_SERVER_CHANNEL_SELECTOR_H_

#include <vector>

#include "aether/server.h"
#include "aether/channel.h"
#include "aether/adapters/adapter.h"

namespace ae {
class Aether;

class ServerChannelSelector {
 public:
  using ChannelsIterator = decltype(Server::channels)::iterator;

  explicit ServerChannelSelector(Ptr<Aether> const& aether, Ptr<Server> server,
                                 Ptr<Adapter> adapter);

  // Get next channel for sending data
  Ptr<class ServerChannelTransport> NextChannel();

 private:
  PtrView<Aether> aether_;
  Ptr<Server> server_;
  Ptr<Adapter> adapter_;

  ChannelsIterator channels_iterator_;
};
}  // namespace ae

#endif  // AETHER_TRANSPORT_SERVER_SERVER_CHANNEL_SELECTOR_H_
