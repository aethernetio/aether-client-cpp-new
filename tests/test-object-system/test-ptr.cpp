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

#include "aether/obj/ptr.h"

namespace ae {
namespace test_ptr {
struct A {
  using ptr = Ptr<A>;

  A() = default;
  virtual ~A() { ++a_destroyed; }

  static inline int a_destroyed = 0;
};

struct B : public A {
  using ptr = Ptr<B>;

  B() : A() {}
  virtual ~B() { ++b_destroyed; }

  static inline int b_destroyed = 0;
};

void test_createPtr() {
  {
    A::ptr a;
    A::ptr a1{nullptr};
    A* a_null = nullptr;
    A::ptr a2{a_null};
  }
  {
    A::ptr a{new A()};
    TEST_ASSERT(a);
  }
  {
    A::ptr a{new A()};
    A::ptr a1{a};
    TEST_ASSERT(a);
    TEST_ASSERT(a1);
  }
  {
    A::ptr a{new A()};
    A::ptr a1{std::move(a)};
    TEST_ASSERT(!a);
    TEST_ASSERT(a1);
  }
  {
    A::ptr a{new A()};
    A::ptr a1{std::move(a)};
    TEST_ASSERT(!a);
    TEST_ASSERT(a1);
  }
  {
    A::ptr a{new B()};
    TEST_ASSERT(a);
  }
  {
    B::ptr b{new B()};
    A::ptr a{b};
    TEST_ASSERT(b);
    TEST_ASSERT(a);
  }
  {
    B::ptr b{new B()};
    A::ptr a{std::move(b)};
    TEST_ASSERT(!b);
    TEST_ASSERT(a);
  }
}

void test_ptrLifeTime() {
  A::a_destroyed = 0;
  {
    A::ptr a{new A()};
    A::ptr a1{new A()};
  }
  TEST_ASSERT_EQUAL(2, A::a_destroyed);

  A::a_destroyed = 0;
  B::b_destroyed = 0;
  {
    // check if parent destructor is called too
    B::ptr b{new B()};
  }
  TEST_ASSERT_EQUAL(1, A::a_destroyed);
  TEST_ASSERT_EQUAL(1, B::b_destroyed);

  A::a_destroyed = 0;
  B::b_destroyed = 0;
  {
    // check if parent destructor is called too
    A::ptr b{new B()};
  }
  TEST_ASSERT_EQUAL(1, A::a_destroyed);
  TEST_ASSERT_EQUAL(1, B::b_destroyed);
}

void test_ptrAssignment() {
  A::a_destroyed = 0;
  {
    A::ptr a{new A()};
    A::ptr a1{};
    a1 = a;
    TEST_ASSERT(a);
    TEST_ASSERT(a1);
  }
  TEST_ASSERT_EQUAL(1, A::a_destroyed);

  A::a_destroyed = 0;
  {
    A::ptr a{new A()};
    A::ptr a1;
    a1 = std::move(a);
    TEST_ASSERT(!a);
    TEST_ASSERT(a1);
  }
  TEST_ASSERT_EQUAL(1, A::a_destroyed);

  A::a_destroyed = 0;
  B::b_destroyed = 0;
  {
    A::ptr a{new A()};
    B::ptr b = a;
    TEST_ASSERT(a);
    TEST_ASSERT(b);
  }
  TEST_ASSERT_EQUAL(1, A::a_destroyed);
  TEST_ASSERT_EQUAL(0, B::b_destroyed);

  A::a_destroyed = 0;
  B::b_destroyed = 0;
  {
    A::ptr a{new A()};
    B::ptr b{};
    b = a;
    TEST_ASSERT(a);
    TEST_ASSERT(b);
  }
  TEST_ASSERT_EQUAL(1, A::a_destroyed);
  TEST_ASSERT_EQUAL(0, B::b_destroyed);

  A::a_destroyed = 0;
  B::b_destroyed = 0;
  {
    A::ptr a{new A()};
    B::ptr b{};
    b = std::move(a);
    TEST_ASSERT(!a);
    TEST_ASSERT(b);
  }
  TEST_ASSERT_EQUAL(1, A::a_destroyed);
  TEST_ASSERT_EQUAL(0, B::b_destroyed);

  A::a_destroyed = 0;
  B::b_destroyed = 0;
  {
    A::ptr a{new A()};
    B::ptr b{new B()};
    b = std::move(a);
    TEST_ASSERT(!a);
    TEST_ASSERT(b);
  }
  TEST_ASSERT_EQUAL(2, A::a_destroyed);
  TEST_ASSERT_EQUAL(1, B::b_destroyed);
}

void test_ptrCompare() {
  {
    A::ptr a{new A()};
    TEST_ASSERT(a);
    A::ptr a1{new A()};
    TEST_ASSERT(a1);
    TEST_ASSERT(a != a1);
  }

  {
    A::ptr a{new A()};
    TEST_ASSERT(a);
    A::ptr a1 = a;
    TEST_ASSERT(a1);
    TEST_ASSERT(a == a1);
  }

  {
    A::ptr a;
    TEST_ASSERT(!a);
    A::ptr a1 = a;
    TEST_ASSERT(!a1);
  }

  {
    A::ptr a{new A()};
    TEST_ASSERT(a);
    B::ptr b = a;
    TEST_ASSERT(a == b);
  }

  {
    A::ptr a{new A()};
    TEST_ASSERT(a);
    B::ptr b{new B()};
    TEST_ASSERT(a != b);
  }
}
}  // namespace test_ptr
}  // namespace ae

int run_test_ptr() {
  UNITY_BEGIN();
  RUN_TEST(ae::test_ptr::test_createPtr);
  RUN_TEST(ae::test_ptr::test_ptrLifeTime);
  RUN_TEST(ae::test_ptr::test_ptrAssignment);
  RUN_TEST(ae::test_ptr::test_ptrCompare);
  return UNITY_END();
}
