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

#ifndef AETHER_STREAM_API_DEBUG_GATE_H_
#define AETHER_STREAM_API_DEBUG_GATE_H_

#include <string>

#include "aether/stream_api/istream.h"

namespace ae {
class DebugGate : public ByteGate {
 public:
  explicit DebugGate(std::string in_format, std::string out_format);

  ActionView<StreamWriteAction> WriteIn(ByteGate::TypeIn in_data,
                                        TimePoint current_time) override;

  void LinkOut(OutGate& out) override;

 private:
  std::string in_format_;
  std::string out_format_;
};
}  // namespace ae

#endif  // AETHER_STREAM_API_DEBUG_GATE_H_
