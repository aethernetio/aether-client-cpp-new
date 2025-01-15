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

#include "aether/channel.h"

namespace ae {

Channel::Channel(Domain* domain) : Obj{domain} {}

// TODO: find a right place for this
Duration Channel::FirstRequestDuration(TokenType /*adapter_token*/,
                                       TokenType /*location_token*/,
                                       float /*percentile*/) const {
  return std::chrono::milliseconds(100);
}

Duration Channel::RequestDuration(TokenType /*adapter_token*/,
                                  TokenType /*location_token*/,
                                  float /*percentile*/) const {
  return std::chrono::milliseconds(100);
}

Duration Channel::ConnectionDuration(TokenType /*adapter_token*/,
                                     TokenType /*location_token*/,
                                     float /*percentile*/) const {
  return std::chrono::milliseconds(100);
}
}  // namespace ae
