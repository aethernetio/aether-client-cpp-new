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

#ifndef AETHER_EVENTS_EVENT_HANDLER_H_
#define AETHER_EVENTS_EVENT_HANDLER_H_

#include <utility>
#include <functional>

namespace ae {
class IEventHandler {
 public:
  virtual ~IEventHandler() = default;

  virtual bool is_alive() const = 0;
  virtual void set_dead() = 0;
  virtual void set_once() = 0;
};

template <typename TSignature>
class EventHandler;

/**
 * \brief RAII object to store event handler callback
 */
template <typename... TArgs>
class EventHandler<void(TArgs...)> : public IEventHandler {
 public:
  template <typename TCallable>
  explicit EventHandler(TCallable cb) : cb_(std::move(cb)) {}

  EventHandler(EventHandler const&) = delete;
  EventHandler(EventHandler&&) noexcept = delete;

  ~EventHandler() override = default;

  void invoke(TArgs... args) {
    if (alive_) {
      cb_(std::forward<TArgs>(args)...);
    }
    if (once_) {
      alive_ = false;
    }
  }

  bool is_alive() const override { return alive_; }
  void set_dead() override { alive_ = false; }
  void set_once() override { once_ = true; }

 private:
  bool alive_{true};  //< handler is alive
  bool once_{false};  //< event handler should be called only once
  std::function<void(TArgs...)> cb_;
};
}  // namespace ae

#endif  // AETHER_EVENTS_EVENT_HANDLER_H_
