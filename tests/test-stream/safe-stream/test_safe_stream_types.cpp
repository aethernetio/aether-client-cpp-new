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
#include "aether/stream_api/safe_stream/sending_chunk_list.h"

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

void test_SendingChunkList() {
  constexpr SafeStreamRingIndex::type window_size = 100;

  SendingChunkList chunk_list{window_size};

  // add three chunks
  chunk_list.Register(SafeStreamRingIndex{0}, SafeStreamRingIndex{5}, Now());
  chunk_list.Register(SafeStreamRingIndex{6}, SafeStreamRingIndex{10}, Now());
  chunk_list.Register(SafeStreamRingIndex{11}, SafeStreamRingIndex{20}, Now());

  TEST_ASSERT_EQUAL(3, chunk_list.size());
  // merge two chunks
  chunk_list.Register(SafeStreamRingIndex{0}, SafeStreamRingIndex{10}, Now());
  TEST_ASSERT_EQUAL(2, chunk_list.size());
  // merge again
  chunk_list.Register(SafeStreamRingIndex{0}, SafeStreamRingIndex{20}, Now());
  TEST_ASSERT_EQUAL(1, chunk_list.size());
  // register a smaller chunk
  chunk_list.Register(SafeStreamRingIndex{0}, SafeStreamRingIndex{10}, Now());
  TEST_ASSERT_EQUAL(2, chunk_list.size());
  // register a smaller between two chunks
  chunk_list.Register(SafeStreamRingIndex{8}, SafeStreamRingIndex{14}, Now());
  TEST_ASSERT_EQUAL(3, chunk_list.size());
  auto& chunk = chunk_list.front();
  TEST_ASSERT(SafeStreamRingIndex{15} == chunk.begin_offset);
  TEST_ASSERT(SafeStreamRingIndex{20} == chunk.end_offset);
  chunk_list.RemoveUpTo(SafeStreamRingIndex{7});
  TEST_ASSERT_EQUAL(2, chunk_list.size());
  chunk_list.RemoveUpTo(SafeStreamRingIndex{20});
  TEST_ASSERT(chunk_list.empty());
}
}  // namespace ae::test_safe_stream_types

int test_safe_stream_types() {
  UNITY_BEGIN();
  RUN_TEST(ae::test_safe_stream_types::test_OffsetRange);
  RUN_TEST(ae::test_safe_stream_types::test_SendingChunkList);
  return UNITY_END();
}
