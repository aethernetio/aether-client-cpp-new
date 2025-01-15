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

#include "aether/stream_api/sized_packet_stream.h"

#include <utility>

#include "aether/mstream.h"
#include "aether/mstream_buffers.h"

namespace ae {

ActionView<StreamWriteAction> SizedPacketGate::WriteIn(DataBuffer buffer,
                                                       TimePoint current_time) {
  assert(out_);

  PacketSize size = buffer.size();
  DataBuffer write_buffer;
  auto buffer_writer = VectorWriter<PacketSize>(buffer);
  auto os = omstream{buffer_writer};
  os << size;
  os.write(buffer.data(), buffer.size());

  return out_->WriteIn(std::move(write_buffer), current_time);
}

static constexpr std::size_t kSizedPacketOverhead = 4;  // max for packet size

std::size_t SizedPacketGate::max_write_in_size() const {
  assert(out_);
  return out_->max_write_in_size() - kSizedPacketOverhead;
}

void SizedPacketGate::LinkOut(OutGate& out) {
  out_ = &out;
  out_data_subscription_ =
      out.out_data_event().Subscribe([this](DataBuffer const& buffer) {
        data_packet_collector_.AddData(buffer);

        for (auto packet = data_packet_collector_.PopPacket(); !packet.empty();
             packet = data_packet_collector_.PopPacket()) {
          out_data_event_.Emit(packet);
        }
      });

  gate_update_subscription_ = out.gate_update_event().Subscribe(
      [this]() { gate_update_event_.Emit(); });

  gate_update_event_.Emit();
}

}  // namespace ae
