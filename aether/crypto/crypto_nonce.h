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

#ifndef AETHER_CRYPTO_CRYPTO_NONCE_H_
#define AETHER_CRYPTO_CRYPTO_NONCE_H_

#include <array>
#include <cstdint>

#include "aether/config.h"

#if AE_CRYPTO_SYNC == AE_CHACHA20_POLY1305
// too long include string
#  include "third_party/libsodium/src/libsodium/include/sodium/\
crypto_aead_chacha20poly1305.h"  // " this helps ide to parse following quotes

#endif

namespace ae {
#if AE_CRYPTO_SYNC == AE_CHACHA20_POLY1305
static constexpr auto kNonceSize = crypto_aead_chacha20poly1305_NPUBBYTES;
struct CryptoNonceChacha20Poly1305
    : public std::array<std::uint8_t, kNonceSize> {
  void Next();
  void Init();

  template <typename T>
  void Serializator(T& s) {
    s& static_cast<std::array<std::uint8_t, kNonceSize>&>(*this);
  }
};
#endif

struct CryptoNonceEmpty {
  void Next();
  void Init();

  template <typename T>
  void Serializator(T& /* s */) {}
};

struct CryptoNonce : public
#if AE_CRYPTO_SYNC == AE_CHACHA20_POLY1305
                     CryptoNonceChacha20Poly1305
#else
                     CryptoNonceEmpty
#endif
{
};

}  // namespace ae

#endif  // AETHER_CRYPTO_CRYPTO_NONCE_H_
