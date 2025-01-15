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

#include "aether/methods/server_reg_api/server_registration_api.h"
#if AE_SUPPORT_REGISTRATION
#  include <utility>

#  include "aether/methods/client_reg_api/client_reg_api.h"

namespace ae {
void ServerRegistrationApi::Pack(Registration&& message, ApiPacker& packer) {
  packer.Pack(Registration::kMessageCode, std::move(message));
}

void ServerRegistrationApi::Pack(RequestProofOfWorkData&& message,
                                 ApiPacker& packer) {
  SendResult::OnResponse(
      packer.Context(), message.request_id,
      [req_id{message.request_id}](ApiParser& parser) {
        parser.Context().MessageNotify(ClientApiRegSafe::ResponseWorkProofData{
            req_id, parser.Extract<PowParams>()});
      });

  packer.Pack(RequestProofOfWorkData::kMessageCode, std::move(message));
}
void ServerRegistrationApi::Pack(ResolveServers&& message, ApiPacker& packer) {
  SendResult::OnResponse(
      packer.Context(), message.request_id,
      [req_id{message.request_id}](ApiParser& parser) {
        parser.Context().MessageNotify(ClientApiRegSafe::ResolveServersResponse{
            req_id, parser.Extract<std::vector<ServerDescriptor>>()});
      });

  packer.Pack(ResolveServers::kMessageCode, std::move(message));
}

}  // namespace ae
#endif
