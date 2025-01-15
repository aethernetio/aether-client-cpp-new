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

#ifndef TESTS_TEST_STREAM_CRYPTO_STREAM_MOCK_KEY_PROVIDER_H_
#define TESTS_TEST_STREAM_CRYPTO_STREAM_MOCK_KEY_PROVIDER_H_

#include <utility>

#include "aether/crypto/ikey_provider.h"

namespace ae {
class MockSyncKeyProvider : public ISyncKeyProvider {
 public:
  explicit MockSyncKeyProvider(Key key) : key_{std::move(key)}, nonce_{} {}
  MockSyncKeyProvider(Key key, CryptoNonce nonce)
      : key_{std::move(key)}, nonce_{std::move(nonce)} {}

  Key GetKey() const override { return key_; }

  CryptoNonce const& Nonce() const override { return nonce_; }

 private:
  Key key_;
  CryptoNonce nonce_;
};

class MockAsyncKeyProvider : public IAsyncKeyProvider {
 public:
  MockAsyncKeyProvider(Key public_key, Key secret_key)
      : public_key_{std::move(public_key)},
        secret_key_{std::move(secret_key)} {}

  Key PublicKey() const override { return public_key_; }
  Key SecretKey() const override { return secret_key_; }

 private:
  Key public_key_;
  Key secret_key_;
};

}  // namespace ae

#endif  // TESTS_TEST_STREAM_CRYPTO_STREAM_MOCK_KEY_PROVIDER_H_
