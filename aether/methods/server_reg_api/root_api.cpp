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

#include "aether/methods/server_reg_api/root_api.h"

#if AE_SUPPORT_REGISTRATION
#  include <utility>

#  include "aether/api_protocol/send_result.h"
#  include "aether/methods/client_reg_api/client_reg_api.h"

namespace ae {
void RootApi::Pack(GetAsymmetricPublicKey message, ApiPacker& packer) {
  SendResult::OnResponse(
      packer.Context(), message.request_id,
      [req_id{message.request_id},
       context{&packer.Context()}](ApiParser& parser) {
        context->MessageNotify(ClientApiRegSafe::GetKeysResponse{
            req_id, parser.Extract<SignedKey>()});
      });

  packer.Pack(GetAsymmetricPublicKey::kMessageCode, message);
}

void RootApi::Pack(Enter message, ApiPacker& packer) {
  packer.Pack(Enter::kMessageCode, std::move(message));
}

}  // namespace ae
#endif
