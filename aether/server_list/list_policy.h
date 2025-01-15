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

#ifndef AETHER_SERVER_LIST_LIST_POLICY_H_
#define AETHER_SERVER_LIST_LIST_POLICY_H_

namespace ae {
class Server;
class Channel;

// Policy interface to make server list
class ServeListPolicy {
 public:
  virtual ~ServeListPolicy() = default;
  /**
   * \brief Returns true if left must be placed before right
   * By places before it means that left will be preferred over right.
   * \note Preferred(a, b) must satisfy Compare requirements \see
   * https://en.cppreference.com/w/cpp/named_req/Compare
   */
  virtual bool Preferred(Server const& left, Server const& right) const = 0;

  /**
   * \brief Returns true if item must be filtered out
   */
  virtual bool Filter(Server const& item) const = 0;
};
}  // namespace ae

#endif  // AETHER_SERVER_LIST_LIST_POLICY_H_
