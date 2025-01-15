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

#include "aether/aether.h"
#include "aether/client.h"
#include "aether/global_ids.h"
#include "aether/obj/domain.h"
#include "aether/port/tele_init.h"
#include "aether/literal_array.h"

#include "aether/tele/tele.h"
#include "aether/tele/ios_time.h"

#if (defined(__linux__) || defined(__unix__) || defined(__APPLE__) || \
     defined(__FreeBSD__) || defined(_WIN64) || defined(_WIN32))
#  include "aether/port/file_systems/file_system_std.h"
#  include "aether/adapters/ethernet.h"
#elif (defined(ESP_PLATFORM))
#  include "aether/port/file_systems/file_system_ram.h"
#  include "aether/adapters/esp32_wifi.h"
#endif

#include "send_messages_bandwidth/common/receiver.h"
#include "send_messages_bandwidth/common/test_action.h"

namespace ae::bench {

static constexpr char WIFI_SSID[] = "Test123";
static constexpr char WIFI_PASS[] = "Test123";

int test_receiver_bandwidth() {
  TeleInit::Init();
  AE_TELE_ENV();

  auto fs =
#if (defined(__linux__) || defined(__unix__) || defined(__APPLE__) || \
     defined(__FreeBSD__) || defined(_WIN64) || defined(_WIN32))
      ae::FileSystemStdFacility{};
#elif ((defined(ESP_PLATFORM)))
      ae::FileSystemRamFacility{};
#endif

#if AE_DISTILLATION
  {
    fs.remove_all();
    auto domain = Domain{TimePoint::clock::now(), fs};
    auto aether = domain.CreateObj<ae::Aether>(GlobalId::kAether);

#  if (defined(__linux__) || defined(__unix__) || defined(__APPLE__) || \
       defined(__FreeBSD__) || defined(_WIN64))
    EthernetAdapter::ptr adapter = domain.CreateObj<EthernetAdapter>(
        GlobalId::kEthernetAdapter, aether, aether->poller);
#  elif ((defined(ESP_PLATFORM)))
    Esp32WifiAdapter::ptr adapter = domain.CreateObj<Esp32WifiAdapter>(
        GlobalId::kEsp32WiFiAdapter, aether, aether->poller,
        std::string(WIFI_SSID), std::string(WIFI_PASS));
#  endif

    adapter.SetFlags(ae::ObjFlags::kUnloadedByDefault);
    aether->adapter_factories.emplace_back(std::move(adapter));

#  if AE_SIGNATURE == AE_ED25519
    aether->crypto->signs_pk_[ae::SignatureMethod::kEd25519] =
        ae::SodiumSignPublicKey{
            ae::MakeLiteralArray("4F202A94AB729FE9B381613AE77A8A7D89EDAB9299C33"
                                 "20D1A0B994BA710CCEB")};

#  elif AE_SIGNATURE == AE_HYDRO_SIGNATURE
    aether->crypto->signs_pk_[ae::SignatureMethod::kHydroSignature] =
        ae::HydrogenSignPublicKey{
            ae::MakeLiteralArray("883B4D7E0FB04A38CA12B3A451B00942048858263EE6E"
                                 "6D61150F2EF15F40343")};
#  endif  // AE_SIGNATURE == AE_ED25519

#  if AE_SUPPORT_REGISTRATION
    // localhost
    aether->registration_cloud->AddServerSettings(IpAddressPortProtocol{
        {IpAddress{IpAddress::Version::kIpV4, {127, 0, 0, 1}}, 9010},
        Protocol::kTcp});
    // cloud address
    aether->registration_cloud->AddServerSettings(IpAddressPortProtocol{
        {IpAddress{IpAddress::Version::kIpV4, {35, 224, 1, 127}}, 9010},
        Protocol::kTcp});
    // cloud name address
    aether->registration_cloud->AddServerSettings(
        NameAddress{"registration.aethernet.io", 9010, Protocol::kTcp});
#  endif  // AE_SUPPORT_REGISTRATION

    domain.SaveRoot(aether);
  }
#endif  // AE_DISTILLATION

  auto domain = Domain{TimePoint::clock::now(), fs};
  auto aether = Aether::ptr{};
  aether.SetId(GlobalId::kAether);
  domain.LoadRoot(aether);
  assert(aether);
  ae::TeleInit::Init(aether);

  ae::Adapter::ptr adapter{domain.LoadCopy(aether->adapter_factories.front())};

  ae::Client::ptr client;

  // register two clients
  if (aether->clients().empty()) {
#if AE_SUPPORT_REGISTRATION
    auto& cloud = aether->registration_cloud;
    domain.LoadRoot(cloud);
    cloud->set_adapter(adapter);

    auto client_register = aether->RegisterClient(
        ae::Uid{ae::MakeLiteralArray("3ac931653d37497087a6fa4ee27744e4")});

    bool register_done = false;
    bool register_failed = false;

    auto reg = client_register->SubscribeOnResult([&](auto const& reg) {
      register_done = true;
      client = reg.client();
    });
    auto reg_failed = client_register->SubscribeOnError(
        [&](auto const&) { register_failed = true; });

    while (!register_done && !register_failed) {
      auto time = ae::TimePoint::clock::now();
      auto next_time = domain.Update(time);
      aether->action_processor->get_trigger().WaitUntil(next_time);

      if (register_failed) {
        AE_TELED_ERROR("Registration failed");
        return -1;
      }
    }
#endif
  } else {
    client = aether->clients()[0];
  }

  // Set adapter for all clouds in the client to work through.
  client->cloud()->set_adapter(adapter);

  domain.SaveRoot(aether);

  auto action_context = ActionContext{*aether->action_processor};
  auto receiver = MakePtr<Receiver>(action_context, client);

  auto test_action =
      TestAction<Receiver>(action_context, receiver, std::size_t{3000});

  auto test_done = false;
  auto test_failed = false;

  auto result_subscription =
      test_action.SubscribeOnResult([&](auto const& action) {
        auto res_name_table = std::array{
            std::string_view{"1 Byte"}, std::string_view{"10 Bytes"},
            std::string_view{"100 Bytes"}, std::string_view{"1000 Bytes"},
            /* std::string_view{"1500 Bytes"}, */
        };
        auto const& results = action.result_table();

        auto res_string = std::string{};
        for (auto i = 0; i < res_name_table.size(); ++i) {
          res_string += Format("{}:{}\n", res_name_table[i], results[i]);
        }
        AE_TELED_DEBUG("Test results: \n {}", res_string);

        test_done = true;
      });

  auto error_subscription = test_action.SubscribeOnError([&](auto const&) {
    AE_TELED_ERROR("Test failed");
    test_failed = true;
  });

  AE_TELED_INFO("Receiver prepared for test width uid {}", client->uid());

  while (!(test_done || test_failed)) {
    auto time = ae::TimePoint::clock::now();
    auto next_time = domain.Update(time);
    aether->action_processor->get_trigger().WaitUntil(
        std::min(next_time, time + std::chrono::seconds(5)));
  }

  return 0;
}
}  // namespace ae::bench

int main() { return ae::bench::test_receiver_bandwidth(); }
