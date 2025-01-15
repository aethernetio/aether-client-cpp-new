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

#ifndef AETHER_ACTIONS_ACTION_TRIGGER_H_
#define AETHER_ACTIONS_ACTION_TRIGGER_H_

#include <mutex>
#include <memory>
#include <atomic>
#include <condition_variable>

#include "aether/common.h"

namespace ae {
struct SyncObject {
  // TODO: is this cross-platform?
  std::mutex mutex_;
  std::condition_variable condition_;
  std::atomic<bool> triggered_{false};
};

class ActionTrigger {
  friend void Merge(ActionTrigger& left, ActionTrigger& right);

  std::shared_ptr<SyncObject> sync_object_;

 public:
  ActionTrigger();
  ~ActionTrigger() = default;

  // Wait trigger
  void Wait();
  // Return false on timeout
  bool WaitUntil(TimePoint timeout);

  // call this by action if update required
  void Trigger();
};

// merge to action triggers in one
extern void Merge(ActionTrigger& left, ActionTrigger& right);

}  // namespace ae

#endif  // AETHER_ACTIONS_ACTION_TRIGGER_H_
