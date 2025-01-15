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

#ifndef AETHER_AE_ACTIONS_PULL_H_
#define AETHER_AE_ACTIONS_PULL_H_

#if 0  // disabled temporary

#  include <cstdint>
#  include <memory>
#  include <vector>

#  include "aether/client_connection.h"
#  include "aether/address.h"
#  include "aether/methods/encryption_api.h"
#  include "aether/methods/request.h"
#  include "aether/ae_message.h"
#  include "aether/methods/work_server_api/data_set_api.h"
#  include "aether/actions/action.h"
#  include "aether/methods/client_api/client_safe_api.h"
#  include "aether/events/events.h"

namespace ae {
class Client;

class Pull : public Action<Pull> {
  enum class State {
    MakePullRequest,
    WaitResponse,
    ResponseReceived,
    Repeat,
    ConnectionFailed,
    End,
  };

 public:
  using MessageFn = std::function<void(AeMessage const& message)>;

  // TODO: configure this from client or on runtime
  struct Descriptor {
    float trigger_percentile = 0.95f;
    Protocol use_protocols = Protocol::kAny;
    Duration interval = std::chrono::milliseconds(1000);
    bool enable_push_responses = true;
    uint_fast8_t num_parallel_requests = 1;
  };

 public:
  Pull(Descriptor&& descriptor, Client* client,
       Ptr<ClientConnection> client_connection,
       std::shared_ptr<IEncryptionApiConfig> encryption_api_config);
  ~Pull() override;

  TimePoint Update(TimePoint t) override;

  std::vector<AeMessage> messages_queue;

 private:
  void SetState(State state);

  void PullMessages(TimePoint current_time);
  void OnPullResponse(DataSetApi<AeMessage>::SelectResult const& result);
  void OnPullResponse(DataSetApi<AeMessage>::SelectAllResult const& result_all);
  void OnPullResponse(std::vector<AeMessage> const& messages);

  void OnPushMessage(ClientSafeApi::SendMessage const& message);

  void SendPacket(TimePoint current_time, std::shared_ptr<Request> request);

  Client* client_;
  Descriptor descriptor_;

  Ptr<ClientConnection> client_connection_;
  std::shared_ptr<IEncryptionApiConfig> encryption_config_;

  State state_;
  MessageFn message_function;

  TimePoint last_request_time_;
  Subscription push_message_subscription_;
  Subscription select_result_subscription_;
};
}  // namespace ae

#endif
#endif  // AETHER_AE_ACTIONS_PULL_H_
