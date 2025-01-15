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

#ifndef AETHER_ADAPTERS_PROXY_H_
#define AETHER_ADAPTERS_PROXY_H_

#include "aether/obj/obj.h"
#include "aether/address.h"

namespace ae {

class Proxy : public Obj {
  AE_OBJECT(Proxy, Obj, 0)

 public:
#ifdef AE_DISTILLATION
  Proxy(Domain* domain) : Obj{domain} {}
#endif  // AE_DISTILLATION

#if AE_SUPPORT_PROXY == 1

  template <typename Dnv>
  void Visit(Dnv& dnv) {
    dnv(*base_ptr_);
    // dnv(end_point_, mode_);
  }

  IpAddressPortProtocol end_point_;
  enum class Mode : std::uint8_t{
      kExclusive,  // Adapter works only through proxies.
      kOptional,   // Adapter uses no proxies if possible.
  };
  // [[maybe_unused]] Mode mode_;
#else
  template <typename Dnv>
  void Visit(Dnv&) {}
#endif  // AE_SUPPORT_PROXY == 1
};

}  // namespace ae

#endif  // AETHER_ADAPTERS_PROXY_H_ */
