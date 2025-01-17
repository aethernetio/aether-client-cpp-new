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

#ifndef AETHER_ADAPTERS_ESP32_WIFI_H_
#define AETHER_ADAPTERS_ESP32_WIFI_H_

#if (defined(ESP_PLATFORM))

#  include <string>

#  include "freertos/FreeRTOS.h"
#  include "freertos/task.h"
#  include "freertos/event_groups.h"
#  include "esp_system.h"
#  include "esp_wifi.h"
#  include "esp_event.h"
#  include "esp_log.h"
#  include "nvs_flash.h"

#  include "lwip/err.h"
#  include "lwip/sys.h"

#  include "aether/events/events.h"
#  include "aether/actions/action_list.h"
#  include "aether/events/event_subscription.h"
#  include "aether/adapters/parent_wifi.h"

namespace ae {
class Esp32WifiAdapter : public ParentWifiAdapter {
  AE_OBJECT(Esp32WifiAdapter, ParentWifiAdapter, 0)

  class CreateTransportAction : public ae::CreateTransportAction {
   public:
    // immediately create the transport
    CreateTransportAction(ActionContext action_context,
                          Esp32WifiAdapter* adapter, Obj::ptr aether,
                          IPoller::ptr poller,
                          IpAddressPortProtocol address_port_protocol_);
    // create the transport when wifi is connected
    CreateTransportAction(ActionContext action_context,
                          EventSubscriber<void(bool)> wifi_connected_event,
                          Esp32WifiAdapter* adapter, Obj::ptr aether,
                          IPoller::ptr poller,
                          IpAddressPortProtocol address_port_protocol_);

    TimePoint Update(TimePoint current_time) override;
    Ptr<ITransport> transport() const;

   private:
    void CreateTransport();

    Esp32WifiAdapter* adapter_;
    Obj::ptr aether_;
    IPoller::ptr poller_;
    IpAddressPortProtocol address_port_protocol_;

    bool once_;
    bool failed_;
    Subscription wifi_connected_subscription_;
    Ptr<ITransport> transport_;
  };

  static constexpr int kMaxRetry = 10;

 public:
#  ifdef AE_DISTILLATION
  Esp32WifiAdapter(ObjPtr<Aether> aether, IPoller::ptr poller, std::string ssid,
                   std::string pass, Domain* domain);
  ~Esp32WifiAdapter();
#  endif  // AE_DISTILLATION

  template <typename Dnv>
  void Visit(Dnv& dnv) {
    dnv(*base_ptr_);
  }

  ActionView<ae::CreateTransportAction> CreateTransport(
      IpAddressPortProtocol const& address_port_protocol) override;

  void Update(TimePoint p) override;

 private:
  void Connect(void);
  void DisConnect(void);

  static void EventHandler(void* arg, esp_event_base_t event_base,
                           int32_t event_id, void* event_data);
  void WifiInitSta(void);
  void WifiInitNvs(void);

  esp_netif_t* esp_netif_{};
  bool connected_{false};
  Event<void(bool result)> wifi_connected_event_;
  Ptr<ActionList<CreateTransportAction>> create_transport_actions_;
};
}  // namespace ae

#endif  // (defined(ESP_PLATFORM))

#endif  // AETHER_ADAPTERS_ESP32_WIFI_H_
