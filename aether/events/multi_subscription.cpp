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

#include "aether/events/multi_subscription.h"

#include <algorithm>

namespace ae {

void MultiSubscription::Reset() { subscriptions_.clear(); }

void MultiSubscription::CleanUp() {
  subscriptions_.erase(
      std::remove_if(std::begin(subscriptions_), std::end(subscriptions_),
                     [](auto const& s) { return !s; }),
      std::end(subscriptions_));
}

void MultiSubscription::PushToVector(Subscription&& subscription) {
  subscriptions_.push_back(std::move(subscription));
}

}  // namespace ae
