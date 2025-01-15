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

#ifndef AETHER_ADDRESS_H_
#define AETHER_ADDRESS_H_

#include <cstdint>
#include <string>

#include "aether/config.h"
#include "aether/variant_type.h"

#include "aether/tele/ios.h"

namespace ae {

struct IpAddress {
  enum class Version : std::uint8_t {
    kIpV4,
    kIpV6,
  };

  Version version = Version::kIpV4;
  union {
#if AE_SUPPORT_IPV6 == 1
    std::uint8_t ipv6_value[16];
#endif  // AE_SUPPORT_IPV6 == 1
#if AE_SUPPORT_IPV4 == 1
    std::uint8_t ipv4_value[4];
#endif  // AE_SUPPORT_IPV4 == 1
  } value;

  friend bool operator==(const IpAddress& left, const IpAddress& right);
  friend bool operator!=(const IpAddress& left, const IpAddress& right);
  friend bool operator<(const IpAddress& lef, const IpAddress& right);

  void set_value(const std::uint8_t* val);

  template <typename T>
  void Serializator(T& s) {
    s & version;
    switch (version) {
      case Version::kIpV4:
#if AE_SUPPORT_IPV4 == 1
        s & value.ipv4_value;
#else
        assert(false);
#endif  // AE_SUPPORT_IPV4 == 1
        break;
      case Version::kIpV6:
#if AE_SUPPORT_IPV6 == 1
        s & value.ipv6_value;
#else
        assert(false);
#endif  // AE_SUPPORT_IPV6 == 1
        break;
      default:
        break;
    }
  }
};

template <>
struct PrintToStream<IpAddress> {
  static void Print(std::ostream& s, IpAddress const& t) {
    switch (t.version) {
      case IpAddress::Version::kIpV4: {
#if AE_SUPPORT_IPV4 == 1
        s << std::setfill('0');
        for (std::size_t i = 0; i < 4; i++) {
          s << int{t.value.ipv4_value[i]};
          if (i < 3) {
            s << ".";
          }
        }
        s << std::setfill(' ');
#else
        assert(false);
#endif  // AE_SUPPORT_IPV4 == 1
        break;
      }
      case IpAddress::Version::kIpV6: {
#if AE_SUPPORT_IPV6 == 1
        // TODO: print as conventional IpV6 format
        s << std::setfill('0') << std::hex;
        for (std::size_t i = 0; i < 16; i++) {
          s << t.value.ipv6_value[i];
          if (i < 15) {
            s << ":";
          }
        }
        s << std::setfill(' ') << std::dec;
#else
        assert(false);
#endif  // AE_SUPPORT_IPV6 == 1
        break;
      }
    }
  }
};

struct IpAddressPort {
  template <typename T>
  void Serializator(T& s) {
    s & ip & port;
  }

  IpAddress ip;
  std::uint16_t port;
};

template <>
struct PrintToStream<IpAddressPort> {
  static void Print(std::ostream& s, IpAddressPort const& t) {
    PrintToStream<IpAddress>::Print(s, t.ip);
    s << ":" << t.port;
  }
};

enum class Protocol : std::uint8_t {
  kTcp,
  // TODO: rest does not supported yet
  /*
  kWebSocket,
  kAny,
    kHttp,
    kHttps,
    kUdp, */
};

struct IpAddressPortProtocol : public IpAddressPort {
  template <typename T>
  void Serializator(T& s) {
    IpAddressPort::Serializator(s);
    s & protocol;
  }

  friend bool operator<(const IpAddressPortProtocol& left,
                        const IpAddressPortProtocol& right);

  Protocol protocol{};
};

template <>
struct PrintToStream<IpAddressPortProtocol> {
  static void Print(std::ostream& s, IpAddressPortProtocol const& t) {
    PrintToStream<IpAddressPort>::Print(s, t);
    s << " protocol: " << static_cast<int>(t.protocol);
  }
};

#if AE_SUPPORT_CLOUD_DNS
struct NameAddress {
  std::string name;
  std::uint16_t port;
  Protocol protocol{};

  template <typename T>
  void Serializator(T& s) {
    s & name & port & protocol;
  }
};

template <>
struct PrintToStream<NameAddress> {
  static void Print(std::ostream& s, NameAddress const& t) {
    s << t.name << ":" << t.port
      << " protocol: " << static_cast<int>(t.protocol);
  }
};
#endif

enum class AddressType : std::uint8_t {
  kResolvedAddress,
  kUnresolvedAddress,
};

struct UnifiedAddress : public VariantType<AddressType, IpAddressPortProtocol
#if AE_SUPPORT_CLOUD_DNS
                                           ,
                                           NameAddress
#endif
                                           > {
  using VariantType::VariantType;
};

}  // namespace ae

#endif  // AETHER_ADDRESS_H_
