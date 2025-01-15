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

#include "send_message_delays/api/bench_delays_api.h"

#include <cassert>

namespace ae::bench {
void BenchDelaysApi::LoadFactory(MessageId message_code, ApiParser& parser) {
  switch (message_code) {
    case WarmUp::kMessageCode:
      parser.Load<WarmUp>(*this);
      break;
    case TwoByte::kMessageCode:
      parser.Load<TwoByte>(*this);
      break;
    case TenBytes::kMessageCode:
      parser.Load<TenBytes>(*this);
      break;
    case HundredBytes::kMessageCode:
      parser.Load<HundredBytes>(*this);
      break;
    case ThousandBytes::kMessageCode:
      parser.Load<ThousandBytes>(*this);
      break;
    case ThousandAndHalfBytes::kMessageCode:
      parser.Load<ThousandAndHalfBytes>(*this);
      break;
    default:
      assert(false);
      break;
  }
}
}  // namespace ae::bench
