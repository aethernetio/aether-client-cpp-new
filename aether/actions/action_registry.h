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

#ifndef AETHER_ACTIONS_ACTION_REGISTRY_H_
#define AETHER_ACTIONS_ACTION_REGISTRY_H_

#include <list>
#include <cstddef>

namespace ae {
template <typename T>
class Action;

class IAction;

class ActionRegistry {
 public:
  struct Index {
    std::size_t counter;   //< counter for all references
    IAction* action;       //< pointer to action action object
  };

  using ActionList = std::list<Index>;
  using Iterator = ActionList::iterator;

  // Manage removing action index from list
  class IndexShare {
   public:
    IndexShare();
    IndexShare(Iterator iterator, ActionRegistry* registry_);
    IndexShare(IndexShare const& other);
    IndexShare(IndexShare&& other) noexcept;
    ~IndexShare();

    IndexShare& operator=(IndexShare const& other);
    IndexShare& operator=(IndexShare&& other) noexcept;

    IAction* get();
    IAction const* get() const;

    void Erase();
    Iterator* iterator();

   private:
    Iterator it_;
    ActionRegistry* registry_;
  };

  template <typename T>
  [[nodiscard]] auto Register(Action<T>& action) {
    auto iter = action_list_.insert(std::end(action_list_), Index{{}, &action});
    return IndexShare{iter, this};
  }

  [[nodiscard]] Iterator begin();
  [[nodiscard]] Iterator end();
  std::size_t size() const;

 private:
  ActionList action_list_;
};
}  // namespace ae

#endif  // AETHER_ACTIONS_ACTION_REGISTRY_H_
