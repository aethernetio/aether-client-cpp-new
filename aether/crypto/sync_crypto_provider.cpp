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

#include "aether/crypto/sync_crypto_provider.h"

#include <utility>

#include "aether/crypto/key.h"

#include "aether/crypto/sodium/sodium_sync_crypto_provider.h"
#include "aether/crypto/hydrogen/hydro_sync_crypto_provider.h"

namespace ae {

namespace _sync_internal {
template <typename KeyType>
Ptr<IEncryptProvider> CreateEncryptImpl(
    KeyType const&, Ptr<ISyncKeyProvider> /* key_provider */) {
  assert(false);
  return {};
}

template <typename KeyType>
Ptr<IDecryptProvider> CreateDecryptImpl(
    KeyType const&, Ptr<ISyncKeyProvider> /* key_provider */) {
  assert(false);
  return {};
}

#if AE_CRYPTO_SYNC == AE_CHACHA20_POLY1305

template <>
Ptr<IEncryptProvider> CreateEncryptImpl(SodiumChachaKey const&,
                                        Ptr<ISyncKeyProvider> key_provider) {
  return MakePtr<SodiumSyncEncryptProvider>(std::move(key_provider));
}

template <>
Ptr<IDecryptProvider> CreateDecryptImpl(SodiumChachaKey const&,
                                        Ptr<ISyncKeyProvider> key_provider) {
  return MakePtr<SodiumSyncDecryptProvider>(std::move(key_provider));
}
#endif

#if AE_CRYPTO_SYNC == AE_HYDRO_CRYPTO_SK
template <>
Ptr<IEncryptProvider> CreateEncryptImpl(HydrogenSecretBoxKey const&,
                                        Ptr<ISyncKeyProvider> key_provider) {
  return MakePtr<HydroSyncEncryptProvider>(std::move(key_provider));
}

template <>
Ptr<IDecryptProvider> CreateDecryptImpl(HydrogenSecretBoxKey const&,
                                        Ptr<ISyncKeyProvider> key_provider) {
  return MakePtr<HydroSyncDecryptProvider>(std::move(key_provider));
}
#endif

}  // namespace _sync_internal

SyncEncryptProvider::SyncEncryptProvider(Ptr<ISyncKeyProvider> key_provider) {
  auto key = key_provider->GetKey();
  impl_ = std::visit(
      [&](auto key_type) {
        return _sync_internal::CreateEncryptImpl(key_type, key_provider);
      },
      key);
  assert(impl_);
}

DataBuffer SyncEncryptProvider::Encrypt(DataBuffer const& data) {
  return impl_->Encrypt(data);
}

std::size_t SyncEncryptProvider::EncryptOverhead() const {
  return impl_->EncryptOverhead();
}

SyncDecryptProvider::SyncDecryptProvider(Ptr<ISyncKeyProvider> key_provider) {
  auto key = key_provider->GetKey();
  impl_ = std::visit(
      [&](auto key_type) {
        return _sync_internal::CreateDecryptImpl(key_type, key_provider);
      },
      key);
  assert(impl_);
}

DataBuffer SyncDecryptProvider::Decrypt(DataBuffer const& data) {
  return impl_->Decrypt(data);
}

}  // namespace ae
