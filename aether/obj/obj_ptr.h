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

#ifndef AETHER_OBJ_OBJ_PTR_H_
#define AETHER_OBJ_OBJ_PTR_H_

#include <cstddef>
#include <new>
#include <type_traits>
#include <utility>
#include <functional>

#include "aether/obj/domain_tree.h"
#include "aether/obj/ptr.h"
#include "aether/obj/obj_id.h"

namespace ae {
class Obj;

template <typename T, typename _ = void>
struct IsObjType : std::false_type {};

template <typename T>
struct IsObjType<T, std::void_t<decltype(T::kClassId)>> : std::true_type {};

class ObjectPtrBase {
 public:
  ObjectPtrBase();
  explicit ObjectPtrBase(Obj* ptr);

  ObjectPtrBase(ObjectPtrBase const& ptr) noexcept;
  ObjectPtrBase(ObjectPtrBase&& ptr) noexcept;

  void SetId(ObjId id);
  void SetFlags(ObjFlags flags);

  ObjId GetId() const;
  ObjFlags GetFlags() const;

  // Find Ptr in domain by ObjId
  template <typename T>
  static Ptr<T> FindObjPtr(T* ptr) {
    return FindObjPtr(static_cast<Obj*>(ptr));
  }

  static Ptr<Obj> FindObjPtr(Obj* ptr);

 protected:
  ObjectPtrBase& operator=(Obj* ptr) noexcept;
  ObjectPtrBase& operator=(ObjectPtrBase const& ptr) noexcept;
  ObjectPtrBase& operator=(ObjectPtrBase&& ptr) noexcept;

 private:
  Obj* ptr_ = nullptr;
  ObjId id_;
  ObjFlags flags_;
};

template <typename T>
class ObjPtr : public Ptr<T>, public ObjectPtrBase {
  template <typename U>
  friend class ObjPtr;

 public:
  ObjPtr() noexcept : Ptr<T>(), ObjectPtrBase() {}
  explicit ObjPtr(std::nullptr_t) noexcept : Ptr<T>(nullptr), ObjectPtrBase() {}

  // TODO: maybe private
  ObjPtr(void* ptr, void (*deleter)(void* ptr)) noexcept
      : Ptr<T>(static_cast<T*>(ptr), deleter),
        ObjectPtrBase(static_cast<Obj*>(ptr)) {}

  explicit ObjPtr(T* ptr) noexcept : Ptr<T>(ptr), ObjectPtrBase(ptr) {}

  template <typename U, std::enable_if_t<IsAbleToCast<T, U>::value, int> = 0>
  explicit ObjPtr(U* ptr) noexcept : Ptr<T>(ptr), ObjectPtrBase(ptr) {}

  ObjPtr(ObjPtr const& ptr) noexcept : Ptr<T>(ptr), ObjectPtrBase(ptr) {}
  ObjPtr(ObjPtr&& ptr) noexcept
      : Ptr<T>(std::move(ptr)), ObjectPtrBase(std::move(ptr)) {}

  ObjPtr(Ptr<T> const& ptr) noexcept
      : Ptr<T>(ptr), ObjectPtrBase(Ptr<T>::get()) {}
  ObjPtr(Ptr<T>&& ptr) noexcept
      : Ptr<T>(std::move(ptr)), ObjectPtrBase(Ptr<T>::get()) {}

  template <typename U, std::enable_if_t<IsAbleToCast<T, U>::value, int> = 0>
  ObjPtr(ObjPtr<U> const& ptr) noexcept : Ptr<T>(ptr), ObjectPtrBase(ptr) {}
  template <typename U, std::enable_if_t<IsAbleToCast<T, U>::value, int> = 0>
  ObjPtr(ObjPtr<U>&& ptr) noexcept
      : Ptr<T>(std::move(ptr)), ObjectPtrBase(std::move(ptr)) {}

  ObjPtr& operator=(T* ptr) noexcept {
    Ptr<T>::operator=(ptr);
    ObjectPtrBase::operator=(ptr);
    return *this;
  }
  template <typename U, std::enable_if_t<IsAbleToCast<T, U>::value, int> = 0>
  ObjPtr& operator=(U* ptr) noexcept {
    Ptr<T>::operator=(ptr);
    ObjectPtrBase::operator=(ptr);
    return *this;
  }

  ObjPtr& operator=(ObjPtr const& ptr) noexcept {
    Ptr<T>::operator=(ptr);
    ObjectPtrBase::operator=(ptr);
    return *this;
  }
  ObjPtr& operator=(ObjPtr&& ptr) noexcept {
    Ptr<T>::operator=(std::move(ptr));
    ObjectPtrBase::operator=(std::move(ptr));
    return *this;
  }

  template <typename U, std::enable_if_t<IsAbleToCast<T, U>::value, int> = 0>
  ObjPtr& operator=(ObjPtr<U> const& ptr) noexcept {
    Ptr<T>::operator=(ptr);
    ObjectPtrBase::operator=(ptr);
    return *this;
  }
  template <typename U, std::enable_if_t<IsAbleToCast<T, U>::value, int> = 0>
  ObjPtr& operator=(ObjPtr<U>&& ptr) noexcept {
    Ptr<T>::operator=(std::move(ptr));
    ObjectPtrBase::operator=(std::move(ptr));
    return *this;
  }

  void ReplaceDeleter(void (*deleter)(void* ptr)) noexcept {
    auto& sp = Ptr<T>::shared_pointer_;
    auto* pm = sp.pointer_manager();
    assert(pm);
    pm->ReplaceDeleter(deleter);
  }
};

// allocate memory for T without calling constructor on it
template <typename T>
static ObjPtr<T> AllocateObject() {
  return ObjPtr<T>(::operator new(sizeof(T), std::nothrow), [](auto* ptr) {
    if (ptr) {
      ::operator delete(ptr);
    }
  });
}

// call constructor of T on memory allocated by AllocateObject and stored in
// ptr
template <typename T, typename... TArgs>
static ObjPtr<T> InitObject(ObjPtr<T> ptr, TArgs&&... args) {
  // new ptr but with same shared_pointer_
  auto res = std::move(ptr);
  auto* place = static_cast<void*>(res.get());
  assert(place);
  // call constructor on already allocated memory
  [[maybe_unused]] auto* placement_res =
      new (place) T(std::forward<TArgs>(args)...);
  assert(placement_res);
  // set proper deleter for initialized object
  res.ReplaceDeleter(PtrDefaultDeleter<T>);
  return res;
}

template <typename T>
static ObjPtr<T> SelfObjPtr(T* ptr) {
  return ObjPtr<T>{ObjectPtrBase::FindObjPtr(ptr)};
}
}  // namespace ae

#endif  // AETHER_OBJ_OBJ_PTR_H_
