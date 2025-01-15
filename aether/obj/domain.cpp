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

#include "aether/obj/domain.h"

#include <algorithm>

#include "aether/obj/obj.h"
#include "aether/tele/tele.h"

namespace ae {

Domain::Domain(TimePoint p, IDomainFacility& facility) : facility_(facility) {
  update_time_ = p;
}

TimePoint Domain::Update(TimePoint current_time) {
  update_time_ = current_time;
  auto next_time = current_time + std::chrono::hours(365);
  for (auto& [_, ptr_view] : id_objects_) {
    auto ptr = ptr_view.Lock();
    if (!ptr) {
      continue;
    }
    // TODO: do not call update for someone who is not want it
    ptr->Update(current_time);
    if (ptr->update_time_ > current_time) {
      next_time = std::min(next_time, ptr->update_time_);
    } else if (ptr->update_time_ < current_time) {
#ifdef DEBUG
      AE_TELE_ERROR("Object", "Update returned next time point in the past");
#endif  // DEBUG_
    }
  }
  return next_time;
}

ObjPtr<Obj> Domain::ConstructObj(Factory const& factory, ObjId obj_id) {
  ObjPtr<Obj> o = factory.create();
  AddObject(obj_id, o);
  o.SetId(obj_id);
  o->domain_ = this;
  return o;
}

bool Domain::IsLast(uint32_t class_id) const {
  return registry_.base_to_derived_.find(class_id) ==
         registry_.base_to_derived_.end();
}

bool Domain::IsExisting(uint32_t class_id) const {
  return registry_.IsExisting(class_id);
}

ObjPtr<Obj> Domain::Find(ObjId obj_id) const {
  if (auto it = id_objects_.find(obj_id.id()); it != id_objects_.end()) {
    return it->second.Lock();
  }
  return {};
}

ObjPtr<Obj> Domain::Find(Obj* ptr) const {
  auto it = ptr_objects_.find(static_cast<void const*>(ptr));
  if (it != std::end(ptr_objects_)) {
    return it->second.Lock();
  }
  return {};
}

void Domain::AddObject(ObjId id, ObjPtr<Obj> const& obj) {
  id_objects_.emplace(id.id(), obj);
  ptr_objects_.emplace(obj.get(), obj);
}

void Domain::RemoveObject(Obj* obj) {
  id_objects_.erase(obj->GetId().id());
  ptr_objects_.erase(obj);
}

Factory* Domain::GetMostRelatedFactory(ObjId id) {
  auto classes = facility_.Enumerate(id);

  // Remove all unsupported classes.
  classes.erase(
      std::remove_if(std::begin(classes), std::end(classes),
                     [this](auto const& c) { return !IsExisting(c); }),
      std::end(classes));

  if (classes.empty()) {
    return nullptr;
  }

  // Build inheritance chain.
  // from base to derived.
  std::sort(std::begin(classes), std::end(classes),
            [this](auto left, auto right) {
              if (registry_.GenerationDistance(right, left) > 0) {
                return false;
              } else if (registry_.GenerationDistance(left, right) >= 0) {
                return true;
              }
              // All classes must be in one inheritance chain.
              assert(false);
              return false;
            });

  // Find the Final class for the most derived class provided and create it.
  for (auto& f : registry_.factories_) {
    if (IsLast(f.first)) {
      // check with most derived class
      int distance = registry_.GenerationDistance(classes.back(), f.first);
      if (distance >= 0) {
        return &f.second;
      }
    }
  }

  return nullptr;
}

Factory* Domain::FindClassFactory(Obj const& obj) {
  return registry_.FindFactory(obj.GetClassId());
}

}  // namespace ae
