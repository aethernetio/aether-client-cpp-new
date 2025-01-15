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

#ifndef AETHER_CRYPTO_KEY_GEN_H_
#define AETHER_CRYPTO_KEY_GEN_H_

#include <cstdint>

#include "aether/crypto/key.h"

namespace ae {
// Secret key generation
bool CryptoSyncKeygen(Key& secret_key);

bool CryptoSyncKeyDerive(Key const& master_key, std::uint32_t server_id,
                         std::uint32_t key_number, Key& client_to_server_key,
                         Key& server_to_client_key);
}  // namespace ae

#endif  // AETHER_CRYPTO_KEY_GEN_H_
