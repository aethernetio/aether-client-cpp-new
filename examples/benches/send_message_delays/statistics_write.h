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

#ifndef EXAMPLES_BENCHES_SEND_MESSAGE_DELAYS_STATISTICS_WRITE_H_
#define EXAMPLES_BENCHES_SEND_MESSAGE_DELAYS_STATISTICS_WRITE_H_

#include <vector>
#include <utility>
#include <ostream>

#include "aether/tele/ios.h"

#include "send_message_delays/delay_statistics.h"

namespace ae::bench {
class StatisticsWriteCsv {
  friend struct PrintToStream<StatisticsWriteCsv>;

 public:
  explicit StatisticsWriteCsv(
      std::vector<std::pair<std::string, DurationStatistics>> statistics);

 private:
  std::vector<std::pair<std::string, DurationStatistics>> statistics_;
};
}  // namespace ae::bench

namespace ae {
template <>
struct PrintToStream<bench::StatisticsWriteCsv> {
  static void Print(std::ostream& stream,
                    const bench::StatisticsWriteCsv& value);
};
}  // namespace ae
#endif  // EXAMPLES_BENCHES_SEND_MESSAGE_DELAYS_STATISTICS_WRITE_H_
