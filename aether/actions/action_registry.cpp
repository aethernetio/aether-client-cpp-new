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

#include "aether/actions/action_registry.h"

#include <cassert>

namespace ae {

ActionRegistry::IndexShare::IndexShare() : registry_{nullptr} {}

ActionRegistry::IndexShare::IndexShare(ActionList::iterator iterator,
                                       ActionRegistry* registry_)
    : it_{iterator}, registry_{registry_} {
  ++it_->counter;
}

ActionRegistry::IndexShare::IndexShare(IndexShare const& other)
    : it_{other.it_}, registry_{other.registry_} {
  if (registry_ != nullptr) {
    ++it_->counter;
  }
}

ActionRegistry::IndexShare::IndexShare(IndexShare&& other) noexcept
    : it_{other.it_}, registry_{other.registry_} {
  other.registry_ = nullptr;
}

ActionRegistry::IndexShare::~IndexShare() {
  if (registry_ != nullptr) {
    assert(it_->counter != 0);
    --it_->counter;
    if (it_->counter == 0) {
      registry_->action_list_.erase(it_);
    }
  }
}

ActionRegistry::IndexShare& ActionRegistry::IndexShare::operator=(
    IndexShare const& other) {
  if (this != &other) {
    registry_ = other.registry_;
    it_ = other.it_;
    if (registry_ != nullptr) {
      ++it_->counter;
    }
  }
  return *this;
}

ActionRegistry::IndexShare& ActionRegistry::IndexShare::operator=(
    IndexShare&& other) noexcept {
  if (this != &other) {
    registry_ = other.registry_;
    it_ = other.it_;
    other.registry_ = nullptr;
  }
  return *this;
}

IAction* ActionRegistry::IndexShare::get() {
  if (registry_ != nullptr) {
    return it_->action;
  }
  return nullptr;
}

IAction const* ActionRegistry::IndexShare::get() const {
  if (registry_ != nullptr) {
    return it_->action;
  }
  return nullptr;
}

void ActionRegistry::IndexShare::Erase() {
  if (registry_ != nullptr) {
    it_->action = nullptr;
  }
}

ActionRegistry::Iterator* ActionRegistry::IndexShare::iterator() {
  if (registry_ != nullptr) {
    return &it_;
  }
  return nullptr;
}

ActionRegistry::Iterator ActionRegistry::begin() {
  return std::begin(action_list_);
}

ActionRegistry::Iterator ActionRegistry::end() {
  return std::end(action_list_);
}

std::size_t ActionRegistry::size() const { return action_list_.size(); }

}  // namespace ae
