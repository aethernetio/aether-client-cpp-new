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

#ifndef AETHER_ADAPTERS_REGISTER_WIFI_H_
#define AETHER_ADAPTERS_REGISTER_WIFI_H_

#include "aether/adapters/parent_wifi.h"

namespace ae {
class RegisterWifiAdapter : public ParentWifiAdapter {
  AE_OBJECT(RegisterWifiAdapter, ParentWifiAdapter, 0)

 public:
#ifdef AE_DISTILLATION
  RegisterWifiAdapter(ObjPtr<Aether> aether, IPoller::ptr poller,
                      std::string ssid, std::string pass, Domain* domain);
#endif  // AE_DISTILLATION

  template <typename Dnv>
  void Visit(Dnv& dnv) {
    dnv(*base_ptr_);
  }

  Ptr<ITransport> CreateTransport(
      IpAddressPortProtocol const& address_port_protocol) override;
};
}  // namespace ae

#endif  // AETHER_ADAPTERS_REGISTER_WIFI_H_
