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

#ifndef AETHER_ACTIONS_ACTION_LIST_H_
#define AETHER_ACTIONS_ACTION_LIST_H_

#include <list>

#include "aether/actions/action_view.h"
#include "aether/actions/action_context.h"
#include "aether/events/multi_subscription.h"

namespace ae {
/**
 * \brief Stores created action as list and removes it on finish
 */
template <typename TAction>
class ActionList {
 public:
  explicit ActionList(ActionContext action_context)
      : action_context_{std::move(action_context)} {}

  template <typename... TArgs>
  ActionView<TAction> Emplace(TArgs&&... args) {
    auto it = actions_.emplace(std::end(actions_), action_context_,
                               std::forward<TArgs>(args)...);

    subscriptions_.Push(
        it->FinishedEvent().Subscribe([this, it]() { actions_.erase(it); }));

    return ActionView{*it};
  }

 private:
  ActionContext action_context_;
  std::list<TAction> actions_;
  MultiSubscription subscriptions_;
};

/**
 * \brief Stores provided action and removes it on finish
 */
template <typename TAction>
class ActionStore {
 public:
  ActionView<TAction> Add(TAction&& action) {
    auto it = actions_.emplace(std::end(actions_), std::move(action));

    subscriptions_.Push(
        it->FinishedEvent().Subscribe([this, it]() { actions_.erase(it); }));

    return ActionView{*it};
  }

 private:
  std::list<TAction> actions_;
  MultiSubscription subscriptions_;
};

}  // namespace ae

#endif  // AETHER_ACTIONS_ACTION_LIST_H_
