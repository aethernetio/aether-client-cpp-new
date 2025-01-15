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

#include "aether/transport/server/server_transport_factory.h"

#include <utility>

#include "aether/aether.h"
#include "aether/adapters/adapter.h"

#include "aether/transport/server/server_transport.h"

namespace ae {
ServerTransportFactory::ServerTransportFactory(Ptr<Aether> const& aether,
                                               Ptr<Adapter> adapter)
    : aether_{aether}, adapter_{std::move(adapter)} {}

ServerTransportFactory::~ServerTransportFactory() = default;

Ptr<ITransport> ServerTransportFactory::CreateTransport(Server::ptr server) {
  auto aether_ptr = aether_.Lock();
  assert(aether_ptr);
  return MakePtr<ServerTransport>(aether_ptr, adapter_, std::move(server));
}

}  // namespace ae
