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

#include "aether/server_list/no_filter_server_list_policy.h"

#include "aether/server.h"

namespace ae {
bool NoFilterServerListPolicy::Preferred(Server const& left,
                                         Server const& right) const {
  // simple ordering comparison
  return left.server_id < right.server_id;
}

bool NoFilterServerListPolicy::Filter(Server const& /* info */) const {
  return false;
}

}  // namespace ae
