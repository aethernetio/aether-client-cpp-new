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

#include "aether/stream_api/safe_stream/safe_stream_api.h"

namespace ae {
void SafeStreamApi::LoadFactory(MessageId message_id, ApiParser& parser) {
  switch (message_id) {
    case Close::kMessageCode:
      parser.Load<Close>(*this);
      break;
    case RequestReport::kMessageCode:
      parser.Load<RequestReport>(*this);
      break;
    case PutReport::kMessageCode:
      parser.Load<PutReport>(*this);
      break;
    case Confirm::kMessageCode:
      parser.Load<Confirm>(*this);
      break;
    case RequestRepeat::kMessageCode:
      parser.Load<RequestRepeat>(*this);
      break;
    case Send::kMessageCode:
      parser.Load<Send>(*this);
      break;
    case Repeat::kMessageCode:
      parser.Load<Repeat>(*this);
      break;
    default:
      assert(false);
      break;
  }
}

template <typename TMessage>
void SafeStreamApi::Execute(TMessage&& message, ApiParser& parser) {
  parser.Context().MessageNotify(std::forward<TMessage>(message));
}

}  // namespace ae
