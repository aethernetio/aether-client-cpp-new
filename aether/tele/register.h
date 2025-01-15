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

#ifndef AETHER_TELE_REGISTER_H_
#define AETHER_TELE_REGISTER_H_

#include <array>

#include "aether/crc.h"
#include "aether/tele/declaration.h"

namespace ae::tele {

template <auto CRC, auto Module>
struct TagProvider {
  static constexpr auto kCrcValue = CRC;
  static constexpr auto kModuleValue = Module;
};

template <auto CheckCRC, typename T, std::size_t Size>
constexpr auto CountDuplicates(std::array<T, Size> const& arr) {
  int count = 0;
  for (auto c : arr) {
    if (CheckCRC == c) {
      count++;
    }
  }
  return count - 1;
}

template <auto... CrcsList>
constexpr bool ContainDuplicatesImpl() {
  auto arr = std::array{CrcsList...};
  return ((CountDuplicates<CrcsList>(arr) > 0) || ...);
}

template <auto... CRCs>
constexpr bool ContainDuplicates() {
  return ContainDuplicatesImpl<CRCs...>();
}

template <typename... TArgs>
constexpr auto Registration(TArgs&&...) {
  constexpr auto arr =
      std::array{Tag{TArgs::kCrcValue, TArgs::kModuleValue}...};
  static_assert(!ContainDuplicates<TArgs::kCrcValue...>(),
                "duplication not allowed");
  return arr;
}

template <auto CRC, typename Arr>
constexpr auto GetIndex(Arr const& arr) {
  for (std::size_t i{}; i < arr.size(); i++) {
    if (arr[i].crc_ == CRC) {
      return static_cast<int>(i);
    }
  }
  return -1;
}

template <auto CRC, typename Arr>
constexpr auto GetModule(Arr const& arr) {
  for (std::size_t i{}; i < arr.size(); i++) {
    if (arr[i].crc_ == CRC) {
      return arr[i].module_;
    }
  }
  return Module::underlined_t{Module::kNone};
}

}  // namespace ae::tele

#define AE_TAG(LITERAL, MODULE) \
  ae::tele::TagProvider<crc32::checksum_from_literal(LITERAL), MODULE> {}

#define AE_TAG_INDEX(TAG_LIST, LITERAL) \
  ae::tele::GetIndex<crc32::checksum_from_literal(LITERAL)>(TAG_LIST)

#define AE_TAG_MODULE(TAG_LIST, LITERAL) \
  ae::tele::GetModule<crc32::checksum_from_literal(LITERAL)>(TAG_LIST)

#endif  // AETHER_TELE_REGISTER_H_
