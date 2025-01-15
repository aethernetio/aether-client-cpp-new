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

#ifndef AETHER_TELE_CONFIGS_CONFIG_SELECTOR_H_
#define AETHER_TELE_CONFIGS_CONFIG_SELECTOR_H_

#include "aether/config.h"
#include "aether/tele/configs/sink_null_trap.h"
#include "aether/tele/configs/sink_to_stream.h"
#include "aether/tele/configs/sink_to_statistics_trap.h"

#if AE_TELE_ENABLED == 1

#  if AE_TELE_LOG_CONSOLE == 1
#    define SELECTED_SINK ae::tele::SinkToStatisticsObjectAndProxyToStream
#  else
#    define SELECTED_SINK ae::tele::SinkToStatisticsObject
#  endif
#else
#  define SELECTED_SINK ae::tele::SinkNullTrap
#endif

#endif  // AETHER_TELE_CONFIGS_CONFIG_SELECTOR_H_
