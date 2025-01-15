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

#ifndef AETHER_TELE_IOS_TIME_H_
#define AETHER_TELE_IOS_TIME_H_

#include <chrono>
#include <sstream>
#include <string>

#include "aether/common.h"

#include "third_party/date/include/date/date.h"

namespace ae {
/**
 * \brief Format TimePoint to string.
 * \see https://howardhinnant.github.io/date/date.html#to_stream_formatting
 */
inline std::string FormatTimePoint(std::string_view format,
                                   TimePoint const& time) {
  std::stringstream ss;

  auto ymd = date::year_month_day{date::floor<date::days>(time)};

  auto micro_time = std::chrono::duration_cast<std::chrono::microseconds>(
      time.time_since_epoch());
  auto today_micro_time = micro_time % std::chrono::hours{24};

  auto tod = date::hh_mm_ss{today_micro_time};
  auto fields = date::fields<std::chrono::microseconds>{ymd, tod};
  date::to_stream(ss, format.data(), fields);
  return ss.str();
}

}  // namespace ae
#endif  // AETHER_TELE_IOS_TIME_H_
