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

#include "aether/transport/server/server_channel_selector.h"

#include <utility>

#include "aether/transport/server/server_channel_transport.h"

#include "aether/tele/tele.h"

namespace ae {
ServerChannelSelector::ServerChannelSelector(Ptr<Aether> const& aether,
                                             Ptr<Server> server,
                                             Ptr<Adapter> adapter)
    : aether_{aether},
      server_(std::move(server)),
      adapter_{std::move(adapter)} {
  channels_iterator_ = server_->channels.begin();
}

Ptr<class ServerChannelTransport> ServerChannelSelector::NextChannel() {
  if (channels_iterator_ == server_->channels.end()) {
    AE_TELED_DEBUG("No channels left");
    // call it again to get the first channel
    channels_iterator_ = server_->channels.begin();
    return {};
  }
  auto& channel = *channels_iterator_;
  ++channels_iterator_;

  if (!channel) {
    server_->LoadChannel(channel);
    assert(channel);
  }

  auto aether = aether_.Lock();
  assert(aether);

  return MakePtr<ServerChannelTransport>(aether, adapter_, channel);
}

}  // namespace ae
