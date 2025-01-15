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

#ifndef AETHER_TELE_CONFIGS_CONFIG_PROVIDER_H_
#define AETHER_TELE_CONFIGS_CONFIG_PROVIDER_H_

#include <cstdint>

#include "aether/config.h"

#include "aether/tele/levels.h"
#include "aether/tele/modules.h"

namespace ae::tele {
struct ConfigProvider {
  template <typename T>
  static constexpr bool ContainsFlag(uint32_t value, T flag) {
    return (value & static_cast<uint32_t>(flag)) == static_cast<uint32_t>(flag);
  }

  template <Module::underlined_t module>
  static constexpr bool IsEnabled(uint32_t value) {
    return AE_TELE_ENABLED && ContainsFlag(value, module);
  }

  struct TeleConfig {
    bool count_metrics_{};
    bool time_metrics_{};
    bool index_logs_{};
    bool start_time_logs_{};
    bool level_module_logs_{};
    bool location_logs_{};
    bool name_logs_{};
    bool blob_logs_{};
  };

  struct EnvConfig {
    bool compiler_{};
    bool platform_type_{};
    bool compilation_options_{};
    bool library_version_{};
    bool api_version_{};
    bool cpu_type_{};
    bool custom_data_{};
  };

  template <Level::underlined_t level, Module::underlined_t module>
  static constexpr TeleConfig StaticTeleConfig{
      IsEnabled<module>(AE_TELE_METRICS_MODULES),  // count must be always
                                                   // enabled if metrics enabled
      IsEnabled<module>(AE_TELE_METRICS_MODULES) &&
          IsEnabled<module>(AE_TELE_METRICS_DURATION),  // time metrics
      IsEnabled<level>(AE_TELE_LOG_LEVELS) &&
          IsEnabled<module>(AE_TELE_LOG_MODULES),  // index must be always
                                                   // enabled if log enabled
      IsEnabled<level>(AE_TELE_LOG_LEVELS) &&
          IsEnabled<module>(AE_TELE_LOG_MODULES) &&
          IsEnabled<module>(AE_TELE_LOG_TIME_POINT),  // start time
      IsEnabled<level>(AE_TELE_LOG_LEVELS) &&
          IsEnabled<module>(AE_TELE_LOG_MODULES) &&
          IsEnabled<module>(AE_TELE_LOG_LEVEL_MODULE),  // level and module
      IsEnabled<level>(AE_TELE_LOG_LEVELS) &&
          IsEnabled<module>(AE_TELE_LOG_MODULES) &&
          IsEnabled<module>(AE_TELE_LOG_LOCATION),  // location
      IsEnabled<level>(AE_TELE_LOG_LEVELS) &&
          IsEnabled<module>(AE_TELE_LOG_MODULES) &&
          IsEnabled<module>(AE_TELE_LOG_NAME),  // name
      IsEnabled<level>(AE_TELE_LOG_LEVELS) &&
          IsEnabled<module>(AE_TELE_LOG_MODULES) &&
          IsEnabled<module>(AE_TELE_LOG_BLOB)  // blob
  };

  static constexpr EnvConfig StaticEnvConfig =
      EnvConfig{AE_TELE_COMPILATION_INFO != 0, AE_TELE_COMPILATION_INFO != 0,
                AE_TELE_COMPILATION_INFO != 0, AE_TELE_COMPILATION_INFO != 0,
                AE_TELE_COMPILATION_INFO != 0, AE_TELE_COMPILATION_INFO != 0,
                AE_TELE_RUNTIME_INFO};
};
}  // namespace ae::tele

#endif  // AETHER_TELE_CONFIGS_CONFIG_PROVIDER_H_ */
