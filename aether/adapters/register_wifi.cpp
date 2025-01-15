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

#include "aether/adapters/register_wifi.h"

#include <memory>

#include "aether/transport/low_level/tcp/unix_tcp.h"
#include "aether/transport/low_level/tcp/win_tcp.h"

namespace ae {

#ifdef AE_DISTILLATION
RegisterWifiAdapter::RegisterWifiAdapter(ObjPtr<Aether> aether,
                                         IPoller::ptr poller, std::string ssid,
                                         std::string pass, Domain* domain)
    : ParentWifiAdapter(aether, poller, ssid, pass, domain)
      {}
#endif  // AE_DISTILLATION

Ptr<ITransport> RegisterWifiAdapter::CreateTransport(
    IpAddressPortProtocol const& address_port_protocol) {
#if defined UNIX_TCP_TRANSPORT_ENABLED
  assert(address_port_protocol.protocol == Protocol::kTcp);
  return MakePtr<UnixTcpTransport>(
      *static_cast<Aether::ptr>(aether_)->action_processor, poller_,
      address_port_protocol);
#elif defined WIN_TCP_TRANSPORT_ENABLED
  assert(address_port_protocol.protocol == Protocol::kTcp);
  return MakePtr<WinTcpTransport>(
      *static_cast<Aether::ptr>(aether_)->action_processor, poller_,
      address_port_protocol);
#else
  return {};
#endif
}

}  // namespace ae
