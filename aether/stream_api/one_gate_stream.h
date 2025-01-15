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

#ifndef AETHER_STREAM_API_ONE_GATE_STREAM_H_
#define AETHER_STREAM_API_ONE_GATE_STREAM_H_

#include <type_traits>

#include "aether/stream_api/istream.h"

namespace ae {
template <typename Gate>
class OneGateStream
    : public Stream<typename std::decay_t<Gate>::TypeIn,
                    typename std::decay_t<Gate>::TypeOut,
                    typename std::decay_t<Gate>::OutGate::TypeIn,
                    typename std::decay_t<Gate>::OutGate::TypeOut> {
  using GateType = std::decay_t<Gate>;
  using BaseStream =
      Stream<typename GateType::TypeIn, typename GateType::TypeOut,
             typename GateType::OutGate::TypeIn,
             typename GateType::OutGate::TypeOut>;

 public:
  template <typename G,
            typename _ = std::void_t<typename std::decay_t<G>::TypeIn,
                                     typename std::decay_t<G>::TypeOut>>
  explicit OneGateStream(G&& gate) : gate_{std::forward<G>(gate)} {}

  OneGateStream(OneGateStream const& other) = delete;
  OneGateStream(OneGateStream&& other) noexcept = default;

  typename BaseStream::InGate& in() override { return gate_; }
  void LinkOut(typename BaseStream::OutGate& out_gate) override {
    gate_.LinkOut(out_gate);
  }

 private:
  Gate gate_;
};

template <typename G>
OneGateStream(G&& gate) -> OneGateStream<G>;

}  // namespace ae

#endif  // AETHER_STREAM_API_ONE_GATE_STREAM_H_
