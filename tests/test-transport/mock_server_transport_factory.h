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

#ifndef TESTS_TEST_TRANSPORT_MOCK_SERVER_TRANSPORT_FACTORY_H_
#define TESTS_TEST_TRANSPORT_MOCK_SERVER_TRANSPORT_FACTORY_H_

#include "aether/server_list/server_transport_factory.h"

#include "test-transport/mock_transport.h"

namespace ae {
class MockServerTransportFactory : public IServerTransportFactory {
 public:
  MockServerTransportFactory(ActionContext action_context,
                             ConnectionInfo connection_info)
      : action_context_{action_context}, connection_info_{connection_info} {}

  Ptr<ITransport> CreateTransport(Server::ptr server) override {
    auto con_info = connection_info_;

    auto transport = MakePtr<MockTransport>(action_context_, con_info);
    transport_created_event_.Emit(transport);
    return transport;
  }

  EventSubscriber<void(Ptr<MockTransport> const& transport)>
  transport_created_event() {
    return transport_created_event_;
  }

 private:
  ActionContext action_context_;
  ConnectionInfo connection_info_;

  Event<void(Ptr<MockTransport> const& transport)> transport_created_event_;
};
}  // namespace ae

#endif  // TESTS_TEST_TRANSPORT_MOCK_SERVER_TRANSPORT_FACTORY_H_
