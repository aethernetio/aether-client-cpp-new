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

#include "aether/statistics.h"

namespace ae {

Duration Statistics::FirstRequestDuration(float /*percentile*/) const {
  return std::chrono::milliseconds(100);
}

Duration Statistics::RequestDuration(float /*percentile*/) const {
  return std::chrono::milliseconds(100);
}

Duration Statistics::ConnectionDuration(float /*percentile*/) const {
  return std::chrono::milliseconds(100);
}

#ifdef AE_DISTILLATION
Statistics::Statistics(Domain* domain) : Obj{domain} {
  first_requests_.push_back({ClockType::now(), std::chrono::milliseconds(200)});
  requests_.push_back({ClockType::now(), std::chrono::milliseconds(100)});
}
#endif  // AE_DISTILLATION

}  // namespace ae
