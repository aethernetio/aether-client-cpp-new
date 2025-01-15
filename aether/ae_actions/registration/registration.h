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

#ifndef AETHER_AE_ACTIONS_REGISTRATION_REGISTRATION_H_
#define AETHER_AE_ACTIONS_REGISTRATION_REGISTRATION_H_

#include "aether/config.h"

#if AE_SUPPORT_REGISTRATION

#  include <vector>

#  include "aether/uid.h"
#  include "aether/common.h"
#  include "aether/state_machine.h"
#  include "aether/actions/action.h"
#  include "aether/actions/action_view.h"
#  include "aether/events/multi_subscription.h"

#  include "aether/client.h"

#  include "aether/stream_api/istream.h"
#  include "aether/crypto/ikey_provider.h"

#  include "aether/transport/itransport.h"
#  include "aether/server_list/server_list.h"
#  include "aether/server_list/server_transport_factory.h"

#  include "aether/methods/server_reg_api/server_registration_api.h"
#  include "aether/methods/client_reg_api/client_reg_api.h"
#  include "aether/methods/client_reg_api/client_global_reg_api.h"

namespace ae {

class Aether;

class Registration : public Action<Registration> {
  enum class State : std::uint8_t {
    kConnection,
    kWaitingConnection,
    kConnected,
    kGetKeys,
    kWaitKeys,
    kGetPowParams,
    kMakeRegistration,
    kRequestCloudResolving,
    kRegistered,
    kRegistrationFailed,
  };

 public:
  Registration(ActionContext action_context, PtrView<Aether> aether,
               Uid parent_uid, Client::ptr client);
  ~Registration() override;

  TimePoint Update(TimePoint t) override;

  Client::ptr client() const;

  template <typename Dnv>
  void Visit(Dnv& dnv) {
    dnv(aether_, client_, server_transport_factory_, server_transport_list_,
        current_server_transport_, reg_server_stream_,
        global_reg_server_stream_);
  }

 private:
  void IterateConnection();

  void Connected(TimePoint current_time);

  void GetKeys(TimePoint current_time);
  TimePoint WaitKeys(TimePoint current_time);
  void OnGetKeysResponse(ClientApiRegSafe::GetKeysResponse const& message);
  void RequestPowParams(TimePoint current_time);
  void OnResponsePowParams(
      ClientApiRegSafe::ResponseWorkProofData const& message);
  void MakeRegistration(TimePoint current_time);
  void OnConfirmRegistration(
      ClientGlobalRegApi::ConfirmRegistration const& message);
  void ResolveCloud(TimePoint current_time);
  void OnResolveCloudResponse(
      ClientApiRegSafe::ResolveServersResponse const& message);

  Ptr<ByteStream> CreateRegServerStream(
      StreamId stream_id, Ptr<IAsyncKeyProvider> async_key_provider,
      Ptr<ISyncKeyProvider> sync_key_provider);
  Ptr<ByteStream> CreateGlobalRegServerStream(
      StreamId stream_id, ServerRegistrationApi::Registration message,
      Ptr<IAsyncKeyProvider> global_async_key_provider,
      Ptr<ISyncKeyProvider> global_sync_key_provider);

  PtrView<Aether> aether_;
  Ptr<Client> client_;
  Uid parent_uid_;

  Ptr<IServerTransportFactory> server_transport_factory_;
  Ptr<ServerList> server_transport_list_;
  ServerList::iterator server_transport_list_iterator_;
  Ptr<ITransport> current_server_transport_;

  ProtocolContext protocol_context_;
  ClientApiRegSafe root_api_;

  StateMachine<State> state_;

  Duration response_timeout_;
  TimePoint last_request_time_;

  Key server_pub_key_;
  Key master_key_;
  Uid uid_;
  Uid ephemeral_uid_;
  Key sign_pk_;
  PowParams pow_params_;
  Key aether_global_key_;
  std::vector<ServerId> cloud_;

  Ptr<class RegistrationAsyncKeyProvider> server_async_key_provider_;
  Ptr<class RegistrationSyncKeyProvider> server_sync_key_provider_;

  Ptr<ByteStream> reg_server_stream_;
  Ptr<ByteStream> global_reg_server_stream_;

  ActionView<StreamWriteAction> packet_write_action_;
  Subscription connection_success_subscription_;
  Subscription connection_error_subscription_;
  Subscription receive_data_subscription_;
  Subscription raw_transport_send_action_subscription_;
  Subscription reg_server_write_subscription_;
  MultiSubscription subscriptions_;
  Subscription state_change_subscription_;
};
}  // namespace ae

#endif
#endif  // AETHER_AE_ACTIONS_REGISTRATION_REGISTRATION_H_
