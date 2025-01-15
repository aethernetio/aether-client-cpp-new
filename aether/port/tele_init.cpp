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

#include "aether/port/tele_init.h"

#include <utility>

#include "aether/aether.h"
#include "aether/config.h"

#include "aether/tele/tele.h"
#include "aether/tele/traps/io_stream_traps.h"
#include "aether/tele/configs/sink_to_statistics_trap.h"

namespace ae {

template <typename SelectedSink>
static void TeleSinkInit() {
#if AE_TELE_ENABLED
  using ae::tele::IoStreamTrap;
  using ae::tele::SinkToStatisticsObject;
  using ae::tele::SinkToStatisticsObjectAndProxyToStream;
  using ae::tele::StatisticsObjectAndStreamTrap;
  using ae::tele::statistics::StatisticsTrap;

  if constexpr (std::is_same_v<SelectedSink,
                               SinkToStatisticsObjectAndProxyToStream>) {
    // statistics trap + print logs to iostream
    auto statistics_object_and_stream_trap =
        MakePtr<StatisticsObjectAndStreamTrap>(
            MakePtr<StatisticsTrap>(), MakePtr<IoStreamTrap>(std::cout));

    SelectedSink::InitSink(std::move(statistics_object_and_stream_trap));
  } else if constexpr (std::is_same_v<SelectedSink, SinkToStatisticsObject>) {
    // just statistics trap
    SelectedSink::InitSink(MakePtr<StatisticsTrap>());
  }
#else
  SelectedSink::InitSink(MakePtr<tele::NullTrap>());
#endif
}

template <typename SelectedSink>
static void TeleSinkReInit([[maybe_unused]] Ptr<Aether>& aether) {
#if AE_TELE_ENABLED
  using ae::tele::IoStreamTrap;
  using ae::tele::SinkToStatisticsObject;
  using ae::tele::SinkToStatisticsObjectAndProxyToStream;
  using ae::tele::StatisticsObjectAndStreamTrap;
  using ae::tele::statistics::StatisticsTrap;

  if constexpr (std::is_same_v<SelectedSink, StatisticsObjectAndStreamTrap>) {
    // statistics trap + print logs to iostream
    auto new_tele_trap = aether->tele_statistics()->trap();
    new_tele_trap->MergeStatistics(*SelectedSink::Instance().trap()->first);

    auto statistics_object_and_stream_trap =
        MakePtr<ae::tele::StatisticsObjectAndStreamTrap>(
            std::move(new_tele_trap),
            MakePtr<ae::tele::IoStreamTrap>(std::cout));

    SelectedSink::InitSink(std::move(statistics_object_and_stream_trap));
  } else if constexpr (std::is_same_v<SelectedSink, SinkToStatisticsObject>) {
    // just statistics trap
    auto new_tele_trap = aether->tele_statistics()->trap();
    new_tele_trap->MergeStatistics(*SelectedSink::Instance().trap());

    SelectedSink::InitSink(std::move(new_tele_trap));
  }
#endif
}

void TeleInit::Init() { TeleSinkInit<TELE_SINK>(); }
void TeleInit::Init(Ptr<Aether>& aether) { TeleSinkReInit<TELE_SINK>(aether); }

}  // namespace ae
