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

#include "aether/crypto/async_crypto_provider.h"

#include <variant>
#include <utility>

#include "aether/crypto/key.h"

#include "aether/crypto/icrypto_provider.h"
#include "aether/crypto/sodium/sodium_async_crypto_provider.h"
#include "aether/crypto/hydrogen/hydro_async_crypto_provider.h"

namespace ae {
namespace _async_internal {
template <typename KeyType>
Ptr<IEncryptProvider> CreateEncryptImpl(
    KeyType const&, Ptr<IAsyncKeyProvider> /* key_provider */) {
  assert(false);
  return {};
}

template <typename KeyType>
Ptr<IDecryptProvider> CreateDecryptImpl(
    KeyType const&, Ptr<IAsyncKeyProvider> /* key_provider */) {
  assert(false);
  return {};
}

#if AE_CRYPTO_ASYNC == AE_SODIUM_BOX_SEAL
template <>
Ptr<IEncryptProvider> CreateEncryptImpl(SodiumCurvePublicKey const&,
                                        Ptr<IAsyncKeyProvider> key_provider) {
  return MakePtr<SodiumAsyncEncryptProvider>(std::move(key_provider));
}

template <>
Ptr<IDecryptProvider> CreateDecryptImpl(SodiumCurvePublicKey const&,
                                        Ptr<IAsyncKeyProvider> key_provider) {
  return MakePtr<SodiumAsyncDecryptProvider>(std::move(key_provider));
}
#endif

#if AE_CRYPTO_ASYNC == AE_HYDRO_CRYPTO_PK
template <>
Ptr<IEncryptProvider> CreateEncryptImpl(HydrogenCurvePublicKey const&,
                                        Ptr<IAsyncKeyProvider> key_provider) {
  return MakePtr<HydroAsyncEncryptProvider>(std::move(key_provider));
}

template <>
Ptr<IDecryptProvider> CreateDecryptImpl(HydrogenCurvePublicKey const&,
                                        Ptr<IAsyncKeyProvider> key_provider) {
  return MakePtr<HydroAsyncDecryptProvider>(std::move(key_provider));
}
#endif
}  // namespace _async_internal

AsyncEncryptProvider::AsyncEncryptProvider(
    Ptr<IAsyncKeyProvider> key_provider) {
  auto pub_key = key_provider->PublicKey();
  impl_ = std::visit(
      [&](auto key_type) {
        return _async_internal::CreateEncryptImpl(key_type,
                                                  std::move(key_provider));
      },
      pub_key);

  assert(impl_);
}

DataBuffer AsyncEncryptProvider::Encrypt(DataBuffer const& data) {
  return impl_->Encrypt(data);
}
std::size_t AsyncEncryptProvider::EncryptOverhead() const {
  return impl_->EncryptOverhead();
}

AsyncDecryptProvider::AsyncDecryptProvider(
    Ptr<IAsyncKeyProvider> key_provider) {
  auto pub_key = key_provider->PublicKey();
  impl_ = std::visit(
      [&](auto key_type) {
        return _async_internal::CreateDecryptImpl(key_type,
                                                  std::move(key_provider));
      },
      pub_key);

  assert(impl_);
}

DataBuffer AsyncDecryptProvider::Decrypt(DataBuffer const& data) {
  return impl_->Decrypt(data);
}

}  // namespace ae
