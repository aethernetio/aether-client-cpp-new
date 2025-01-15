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

#include "send_message_delays/statistics_write.h"

namespace ae::bench {
StatisticsWriteCsv::StatisticsWriteCsv(
    std::vector<std::pair<std::string, DurationStatistics>> statistics)
    : statistics_{std::move(statistics)} {}

}  // namespace ae::bench

namespace ae {
void PrintToStream<bench::StatisticsWriteCsv>::Print(
    std::ostream& stream, const bench::StatisticsWriteCsv& value) {
  // print quick statistics
  stream << "test name,max us,99% us,50% us,min us\n";
  for (auto const& [name, statistics] : value.statistics_) {
    stream << name << ',' << statistics.max_value().count() << ','
           << statistics.get_99th_percentile().count() << ','
           << statistics.get_50th_percentile().count() << ','
           << statistics.min_value().count() << '\n';
  }

  stream << "raw results\n";
  // print legend
  stream << "message num";
  for (auto const& [name, _] : value.statistics_) {
    stream << ',' << name;
  }
  stream << '\n';
  // print data
  for (std::size_t i = 0;
       i < value.statistics_.begin()->second.raw_data().size(); ++i) {
    stream << i;
    for (auto const& [_, statistics] : value.statistics_) {
      stream << ',' << statistics.raw_data()[i].second.count();
    }
    stream << '\n';
  }
  stream << '\n';
}
}  // namespace ae
