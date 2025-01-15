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

#ifndef AETHER_TRANSPORT_SERVER_SERVER_TRANSPORT_FACTORY_H_
#define AETHER_TRANSPORT_SERVER_SERVER_TRANSPORT_FACTORY_H_

#include "aether/server_list/server_transport_factory.h"

namespace ae {
class Aether;
class Adapter;

class ServerTransportFactory : public IServerTransportFactory {
 public:
  ServerTransportFactory(Ptr<Aether> const& aether, Ptr<Adapter> adapter);
  ~ServerTransportFactory() override;

  Ptr<ITransport> CreateTransport(Server::ptr server) override;

 private:
  PtrView<Aether> aether_;
  Ptr<Adapter> adapter_;
};
}  // namespace ae
#endif  // AETHER_TRANSPORT_SERVER_SERVER_TRANSPORT_FACTORY_H_
