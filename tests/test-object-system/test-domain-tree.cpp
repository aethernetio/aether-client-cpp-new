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

#include <unity.h>

#include <iostream>
#include <type_traits>
#include <vector>
#include <map>

#include "aether/obj/domain_tree.h"
#include "aether/obj/version_iterator.h"
#include "aether/obj/visitor_policies.h"

struct DummyNv;

struct Base {};

struct A : public Base {
  int a{1};
  float b{2.2};
  std::string c{"hello"};

  template <typename Dn>
  void Visit(Dn& dn) {
    dn(a, b, c);
  }
};

struct B : public Base {
  bool d{true};
  A a{};

  template <typename Dn>
  void Visit(Dn& dn) {
    dn(static_cast<Base&>(*this), d, a);
  }
};

struct F2;

struct F1 {
  F2* f2_{};

  template <typename Dn>
  void Visit(Dn& dn) {
    if (f2_) {
      dn(*f2_);
    }
  }
};
struct F2 {
  F1* f1_{};

  template <typename Dn>
  void Visit(Dn& dn) {
    if (f1_) {
      dn(*f1_);
    }
  }
};

struct F3 {
  std::vector<F1*> f1s_;

  template <typename Dn>
  void Visit(Dn& dn) {
    dn(f1s_);
  }
};

struct F4 {
  F1* f1_{};
  F2* f2_{};
  F3* f3_{};

  template <typename Dnv>
  void Save(ae::Version<1>, Dnv& dnv) {
    dnv(f1_);
  }
  template <typename Dnv>
  void Save(ae::Version<2>, Dnv& dnv) {
    dnv(f2_);
  }
  template <typename Dnv>
  void Save(ae::Version<3>, Dnv& dnv) {
    dnv(f3_);
  }
};

void PrintFunc(A const&) { std::cout << "A" << std::endl; }
void PrintFunc(B const&) { std::cout << "B" << std::endl; }
void PrintFunc(Base const&) { std::cout << "Base" << std::endl; }
template <typename T>
void PrintFunc(T const& v) {
  std::cout << v << std::endl;
}

void test_PrintDomainTree() {
  B b{};
  ae::DomainTree<>::Visit(b, [](auto& v) { PrintFunc(v); });
}

void test_PrintAnBOnly() {
  B b{};
  ae::DomainTree<>::Visit(b, [](auto& v) {
    if constexpr (std::is_same_v<B, std::decay_t<decltype(v)>> ||
                  std::is_same_v<A, std::decay_t<decltype(v)>>) {
      PrintFunc(v);
    }
  });
}

void test_PrintBaseOnly() {
  B b{};
  ae::DomainTree<>::Visit(b,
                          [](Base& v) { std::cout << " Base" << std::endl; });
}

void test_cycleVisit() {
  F1 f1{};
  F2 f2{};
  f1.f2_ = &f2;
  f2.f1_ = &f1;
  int f1_count = 0;
  int f2_count = 0;

  ae::DomainTree<>::Visit(f1, [&f1_count, &f2_count](auto& v) {
    if constexpr (std::is_same_v<F1, std::decay_t<decltype(v)>>) {
      f1_count++;
    } else if constexpr (std::is_same_v<F2, std::decay_t<decltype(v)>>) {
      f2_count++;
    }
  });

  TEST_ASSERT_EQUAL(1, f1_count);
  TEST_ASSERT_EQUAL(1, f2_count);
}
void test_cycleVisitInContainer() {
  using VisitorPolicy = ae::DeepVisitPolicy<>;

  // check all possible HasVisit combinations for DeepVisitPolicy
  static_assert(VisitorPolicy::HasVisitMethod<F1, DummyNv>::value);
  static_assert(VisitorPolicy::HasVisitMethod<F2, DummyNv>::value);

  static_assert(VisitorPolicy::HasVisitMethod<F3, DummyNv>::value);
  static_assert(VisitorPolicy::HasVisitMethod<F3*, DummyNv>::value);
  static_assert(VisitorPolicy::HasVisitMethod<std::vector<F3>, DummyNv>::value);
  static_assert(
      VisitorPolicy::HasVisitMethod<std::vector<F3*>, DummyNv>::value);
  static_assert(
      VisitorPolicy::HasVisitMethod<std::map<int, F3>, DummyNv>::value);
  static_assert(
      VisitorPolicy::HasVisitMethod<std::map<int, F3*>, DummyNv>::value);

  F1 f11{};
  F1 f12{};
  F1 f13{};

  F2 f21{};
  F2 f22{};
  F2 f23{};

  f11.f2_ = &f21;
  f21.f1_ = &f11;
  f12.f2_ = &f22;
  f22.f1_ = &f12;
  f13.f2_ = &f23;
  f23.f1_ = &f13;

  F3 f3{};
  f3.f1s_.push_back(&f11);
  f3.f1s_.push_back(&f12);
  f3.f1s_.push_back(&f13);

  int f1_ref_count = 0;
  int f2_ref_count = 0;

  ae::DomainTree<VisitorPolicy>::Visit(f3, [&](auto const& v) {
    using FType = std::remove_pointer_t<std::decay_t<decltype(v)>>;
    if constexpr (std::is_same_v<F1, FType>) {
      f1_ref_count++;
    } else if constexpr (std::is_same_v<F2, FType>) {
      f2_ref_count++;
    }
  });

  TEST_ASSERT_EQUAL(6, f1_ref_count);
  TEST_ASSERT_EQUAL(3, f2_ref_count);
}

void test_multiplexVisitor() {
  using VisitorPolicy = ae::MultiplexVisitorPolicy<
      ae::DeepVisitPolicy<>,
      ae::DeepVisitPolicy<ae::VersionedSaveVisitorPolicy>>;

  static_assert(VisitorPolicy::HasVisitMethod<F4, DummyNv>::value);
  static_assert(VisitorPolicy::HasVisitMethod<F4*, DummyNv>::value);
  static_assert(VisitorPolicy::HasVisitMethod<std::vector<F4>, DummyNv>::value);
  static_assert(
      VisitorPolicy::HasVisitMethod<std::map<int, F4>, DummyNv>::value);

  F4 f4{};
  F1 f1{};
  F2 f2{};
  F3 f3{};
  f4.f1_ = &f1;
  f4.f2_ = &f2;
  f4.f3_ = &f3;

  F2 f12{};
  f1.f2_ = &f12;
  F1 f21{};
  f12.f1_ = &f21;
  f2.f1_ = &f21;

  F1 f01{};
  f01.f2_ = &f12;
  f3.f1s_.push_back(&f01);

  int f1_ref_count = 0;
  int f2_ref_count = 0;
  int f3_ref_count = 0;
  int f4_ref_count = 0;

  ae::DomainTree<VisitorPolicy>::Visit(f4, [&](auto const& v) {
    using FType = std::remove_pointer_t<std::decay_t<decltype(v)>>;
    if constexpr (std::is_same_v<F4, FType>) {
      f4_ref_count++;
    } else if constexpr (std::is_same_v<F1, FType>) {
      f1_ref_count++;
    } else if constexpr (std::is_same_v<F2, FType>) {
      f2_ref_count++;
    } else if constexpr (std::is_same_v<F3, FType>) {
      f3_ref_count++;
    }
  });

  // not call on self
  TEST_ASSERT_EQUAL(0, f4_ref_count);
  TEST_ASSERT_EQUAL(1, f3_ref_count);
  TEST_ASSERT_EQUAL(3, f2_ref_count);
  TEST_ASSERT_EQUAL(4, f1_ref_count);
}

int run_test_domain_tree() {
  UNITY_BEGIN();
  RUN_TEST(test_PrintDomainTree);
  RUN_TEST(test_PrintAnBOnly);
  RUN_TEST(test_PrintBaseOnly);
  RUN_TEST(test_cycleVisit);
  RUN_TEST(test_cycleVisitInContainer);
  RUN_TEST(test_multiplexVisitor);
  return UNITY_END();
}
