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

#include "unity.h"

#include "aether/stream_api/safe_stream/safe_stream_types.h"

namespace ae::test_safe_stream_types {
void test_OffsetRange() {
  constexpr SafeStreamRingIndex::type window_size = 100;
  {
    constexpr auto half_window_range = OffsetRange{
        SafeStreamRingIndex{25}, SafeStreamRingIndex{75}, window_size};

    TEST_ASSERT_TRUE(half_window_range.Before(SafeStreamRingIndex{76}));
    TEST_ASSERT_TRUE(half_window_range.After(SafeStreamRingIndex{24}));
    TEST_ASSERT_FALSE(half_window_range.Before(SafeStreamRingIndex{75}));
    TEST_ASSERT_FALSE(half_window_range.After(SafeStreamRingIndex{25}));
    TEST_ASSERT_TRUE(half_window_range.InRange(SafeStreamRingIndex{25}));
    TEST_ASSERT_TRUE(half_window_range.InRange(SafeStreamRingIndex{75}));
    TEST_ASSERT_TRUE(half_window_range.InRange(SafeStreamRingIndex{50}));
    TEST_ASSERT_FALSE(half_window_range.InRange(SafeStreamRingIndex{20}));
    TEST_ASSERT_FALSE(half_window_range.InRange(SafeStreamRingIndex{80}));
  }

  constexpr auto window_range = OffsetRange{
      SafeStreamRingIndex{0}, SafeStreamRingIndex{100}, window_size};
  TEST_ASSERT_TRUE(window_range.InRange(SafeStreamRingIndex{0}));
  TEST_ASSERT_TRUE(window_range.InRange(SafeStreamRingIndex{100}));
  TEST_ASSERT_TRUE(window_range.InRange(SafeStreamRingIndex{10}));
  TEST_ASSERT_FALSE(window_range.InRange(SafeStreamRingIndex{110}));
  TEST_ASSERT_TRUE(window_range.Before(SafeStreamRingIndex{110}));
  TEST_ASSERT_TRUE(window_range.After(SafeStreamRingIndex{110}));
}
}  // namespace ae::test_safe_stream_types

int test_safe_stream_types() {
  UNITY_BEGIN();
  RUN_TEST(ae::test_safe_stream_types::test_OffsetRange);
  return UNITY_END();
}
