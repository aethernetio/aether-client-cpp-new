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

#ifndef AETHER_STREAM_API_SAFE_STREAM_SAFE_STREAM_CONFIG_H_
#define AETHER_STREAM_API_SAFE_STREAM_SAFE_STREAM_CONFIG_H_

#include <cstdint>

#include "aether/common.h"
#include "aether/stream_api/safe_stream/safe_stream_types.h"

namespace ae {
struct SafeStreamConfig {
  SafeStreamRingIndex::type buffer_capacity;  //< sending buffer capacity
  SafeStreamRingIndex::type window_size;      //< size of sending window
  SafeStreamRingIndex::type max_data_size;    //< max size of sending data
  std::uint16_t max_repeat_count;  //< max repeat count for sending packet
  Duration wait_confirm_timeout;   //< Timeout for waiting confirmation
  Duration send_confirm_timeout;   //< max time to wait before send confirmation
  Duration send_repeat_timeout;  //< max time to wait before send repeat request
};

}  // namespace ae

#endif  // AETHER_STREAM_API_SAFE_STREAM_SAFE_STREAM_CONFIG_H_
