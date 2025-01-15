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

#ifndef AETHER_CRYPTO_IKEY_PROVIDER_H_
#define AETHER_CRYPTO_IKEY_PROVIDER_H_

#include "aether/crypto/key.h"
#include "aether/crypto/crypto_nonce.h"

namespace ae {
/**
 * \brief Key provider for synchronous algorithms.
 */
class ISyncKeyProvider {
 public:
  virtual ~ISyncKeyProvider() = default;

  virtual Key GetKey() const = 0;
  virtual CryptoNonce const& Nonce() const = 0;
};

/**
 * \brief Key provider for asynchronous algorithms.
 */
class IAsyncKeyProvider {
 public:
  virtual ~IAsyncKeyProvider() = default;

  virtual Key PublicKey() const = 0;
  virtual Key SecretKey() const = 0;
};
}  // namespace ae

#endif  // AETHER_CRYPTO_IKEY_PROVIDER_H_
