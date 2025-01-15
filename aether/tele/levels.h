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

#ifndef AETHER_TELE_LEVELS_H_
#define AETHER_TELE_LEVELS_H_

#include <cstdint>
#include <string>

namespace ae::tele {
struct Level {
  using underlined_t = std::uint8_t;
  enum _ : std::uint8_t {
    kInfo = 1 << 0,
    kWarning = 1 << 1,
    kError = 1 << 2,
    kDebug = 1 << 3,
    kAll = 0xFF,
  };

  static std::string text(underlined_t v) {
    switch (v) {
      case kInfo:
        return "kInfo";
      case kWarning:
        return "kWarning";
      case kError:
        return "kError";
      case kDebug:
        return "kDebug";
    }
    return "";
  }

  operator underlined_t() const noexcept { return value_; }

  underlined_t value_;
};
}  // namespace ae::tele

#endif  // AETHER_TELE_LEVELS_H_ */
