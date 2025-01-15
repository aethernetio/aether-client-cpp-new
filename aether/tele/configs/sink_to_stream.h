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

#ifndef AETHER_TELE_CONFIGS_SINK_TO_STREAM_H_
#define AETHER_TELE_CONFIGS_SINK_TO_STREAM_H_

#include "aether/tele/configs/config_provider.h"
#include "aether/tele/sink.h"
#include "aether/tele/traps/io_stream_traps.h"

namespace ae::tele {
using TeleSinkToStream = TeleSink<IoStreamTrap, ConfigProvider>;
}
#endif  // AETHER_TELE_CONFIGS_SINK_TO_STREAM_H_ */
