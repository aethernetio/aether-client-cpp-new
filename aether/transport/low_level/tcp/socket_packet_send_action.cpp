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

#include "aether/transport/low_level/tcp/socket_packet_send_action.h"

namespace ae {
TimePoint SocketPacketSendAction::Update(TimePoint current_time) {
  if (state_.changed()) {
    switch (state_.Acquire()) {
      case State::kSuccess:
        Action::Result(*this);
        break;
      case State::kFailed:
      case State::kPanic:
      case State::kTimeout:
        Action::Error(*this);
        break;
      case State::kStopped:
        Action::Stop(*this);
        break;
      default:
        break;
    }
  }
  return current_time;
}

void SocketPacketSendAction::Stop() {
  state_.Set(State::kStopped);
  Action::Trigger();
}
}  // namespace ae
