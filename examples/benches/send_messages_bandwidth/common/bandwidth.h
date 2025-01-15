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

#ifndef EXAMPLES_BENCHES_SEND_MESSAGES_BANDWIDTH_COMMON_BANDWIDTH_H_
#define EXAMPLES_BENCHES_SEND_MESSAGES_BANDWIDTH_COMMON_BANDWIDTH_H_

#include <cstddef>
#include <chrono>

#include "aether/common.h"
#include "aether/tele/ios.h"

namespace ae::bench {
using HighResTimePoint =
    std::chrono::time_point<std::chrono::high_resolution_clock>;

class Bandwidth {
 public:
  Bandwidth(Duration duration, std::size_t message_count,
            std::size_t message_size);

  double bandwidth;  //< bytes per second
  std::size_t message_count;
  Duration duration;
};
}  // namespace ae::bench

namespace ae {
template <>
struct PrintToStream<bench::Bandwidth> {
  static void Print(std::ostream& s, bench::Bandwidth const& b) {
    s << "Bandwidth: " << b.bandwidth << " bytes/s"
      << " Message count: " << b.message_count
      << " Duration: " << b.duration.count() << " us";
  }
};
}  // namespace ae

#endif  // EXAMPLES_BENCHES_SEND_MESSAGES_BANDWIDTH_COMMON_BANDWIDTH_H_