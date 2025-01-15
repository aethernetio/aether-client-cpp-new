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

#include "send_messages_bandwidth/common/bandwidth_api.h"

namespace ae::bench {
void BandwidthApi::LoadFactory(MessageId message_code, ApiParser& api_parser) {
  switch (message_code) {
    case Handshake::kMessageCode:
      api_parser.Load<Handshake>(*this);
      break;
    case Sync::kMessageCode:
      api_parser.Load<Sync>(*this);
      break;
    case WarmUp::kMessageCode:
      api_parser.Load<WarmUp>(*this);
      break;
    case OneByte::kMessageCode:
      api_parser.Load<OneByte>(*this);
      break;
    case TenBytes::kMessageCode:
      api_parser.Load<TenBytes>(*this);
      break;
    case HundredBytes::kMessageCode:
      api_parser.Load<HundredBytes>(*this);
      break;
    case ThousandBytes::kMessageCode:
      api_parser.Load<ThousandBytes>(*this);
      break;
    case VarMessageSize::kMessageCode:
      api_parser.Load<VarMessageSize>(*this);
      break;
    default:
      if (!ExtendsApi::LoadExtend(message_code, api_parser)) {
        assert(false);
      }
      break;
  }
}

template <typename T>
void BandwidthApi::Execute(T&& message, ApiParser& api_parser) {
  api_parser.Context().MessageNotify(std::forward<T>(message));
}
}  // namespace ae::bench