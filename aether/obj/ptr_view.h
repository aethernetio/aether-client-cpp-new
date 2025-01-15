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

#ifndef AETHER_OBJ_PTR_VIEW_H_
#define AETHER_OBJ_PTR_VIEW_H_

#include "aether/obj/ptr.h"

namespace ae {

template <typename T>
class PtrView {
  template <typename U>
  friend class PtrView;

 public:
  PtrView() noexcept : shared_pointer_{} {}
  PtrView(PtrView const& ptr_view) noexcept
      : shared_pointer_{ptr_view.shared_pointer_} {}

  PtrView(PtrView&& ptr_view) noexcept
      : shared_pointer_{ptr_view.shared_pointer_} {
    ptr_view.shared_pointer_.Leak();
  }

  template <typename U, std::enable_if_t<IsAbleToCast<T, U>::value, int> = 0>
  PtrView(PtrView<U> const& ptr_view) noexcept
      : shared_pointer_{ptr_view.shared_pointer_} {}

  template <typename U, std::enable_if_t<IsAbleToCast<T, U>::value, int> = 0>
  PtrView(PtrView<U>&& ptr_view) noexcept
      : shared_pointer_{ptr_view.shared_pointer_} {
    ptr_view.shared_pointer_.Leak();
  }

  template <typename U, std::enable_if_t<IsAbleToCast<T, U>::value, int> = 0>
  PtrView(Ptr<U> const& ptr) noexcept : shared_pointer_{ptr.shared_pointer_} {}

  PtrView& operator=(PtrView const& ptr_view) noexcept {
    if (this != &ptr_view) {
      Reset();
      shared_pointer_ = ptr_view.shared_pointer_;
    }
    return *this;
  }

  PtrView& operator=(PtrView&& ptr_view) noexcept {
    if (this != &ptr_view) {
      Reset();
      shared_pointer_ = ptr_view.shared_pointer_;
      ptr_view.shared_pointer_.Leak();
    }
    return *this;
  }

  template <typename U, std::enable_if_t<IsAbleToCast<T, U>::value, int> = 0>
  PtrView& operator=(PtrView<U> const& ptr_view) noexcept {
    Reset();
    shared_pointer_ = ptr_view.shared_pointer_;
    return *this;
  }

  template <typename U, std::enable_if_t<IsAbleToCast<T, U>::value, int> = 0>
  PtrView& operator=(PtrView<U>&& ptr_view) noexcept {
    Reset();
    shared_pointer_ = ptr_view.shared_pointer_;
    ptr_view.shared_pointer_.Leak();
    return *this;
  }

  PtrView& operator=(Ptr<T> const& ptr) noexcept {
    Reset();
    shared_pointer_ = ptr.shared_pointer_;
    return *this;
  }

  Ptr<T> Lock() const noexcept {
    if (operator bool()) {
      return Ptr<T>(shared_pointer_);
    }
    return Ptr<T>{nullptr};
  }

  explicit operator bool() const noexcept {
    if (auto* pm = shared_pointer_.pointer_manager(); pm) {
      return pm->ref_counter() != 0;
    }
    return false;
  }

  void Reset() noexcept { shared_pointer_ = {}; }

 private:
  SharedPointer<T, PtrDnv> shared_pointer_;
};

template <typename U>
PtrView(Ptr<U> const& ptr) -> PtrView<U>;

}  // namespace ae

#endif  // AETHER_OBJ_PTR_VIEW_H_
