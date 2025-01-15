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

#include "aether/stream_api/safe_stream/safe_stream_types.h"
#include "aether/stream_api/safe_stream/sending_data_action.h"
#include "aether/transport/data_buffer.h"
#include "unity_internals.h"

namespace ae::test_send_data_action {

static constexpr char test_data[] =
    "The old lightbulb flickered one last time, then darkness fell. It had "
    "illuminated the room for years, witnessing countless moments. Now, "
    "forgotten and replaced, it lay cold in the trash.";

void test_SendingData() {
  auto empty_data = SendingData{
      SafeStreamRingIndex{10},
      DataBuffer{},
  };

  auto empty_range = empty_data.OffsetRange();
  TEST_ASSERT_EQUAL(10,
                    static_cast<SafeStreamRingIndex::type>(empty_range.first));

  TEST_ASSERT_EQUAL(10,
                    static_cast<SafeStreamRingIndex::type>(empty_range.second));

  auto data =
      std::vector<std::uint8_t>{test_data, test_data + sizeof(test_data)};

  auto sending_data = SendingData{
      SafeStreamRingIndex{0},
      DataBuffer{std::move(data)},
  };

  auto offset_range = sending_data.OffsetRange();

  TEST_ASSERT_EQUAL(0,
                    static_cast<SafeStreamRingIndex::type>(offset_range.first));
  TEST_ASSERT_EQUAL(
      sending_data.data.size() - 1,
      static_cast<SafeStreamRingIndex::type>(offset_range.second));

  auto sending_data2 = SendingData{
      SafeStreamRingIndex{250},
      DataBuffer{std::move(sending_data.data)},
  };

  auto offset_range2 = sending_data2.OffsetRange();

  TEST_ASSERT_EQUAL(
      250, static_cast<SafeStreamRingIndex::type>(offset_range2.first));
  TEST_ASSERT_EQUAL(
      250 + sending_data2.data.size() - 1,
      static_cast<SafeStreamRingIndex::type>(offset_range2.second));
}

void test_SendingDataIter() {
  auto data =
      std::vector<std::uint8_t>{test_data, test_data + sizeof(test_data)};

  auto sending_data = SendingData{
      SafeStreamRingIndex{0},
      DataBuffer{std::move(data)},
  };

  auto iter1 = sending_data.Iter(SafeStreamRingIndex{0}, 20);
  auto iter2 = sending_data.Iter(SafeStreamRingIndex{20}, 30);
  auto iter3 = sending_data.Iter(SafeStreamRingIndex{50}, sizeof(test_data));

  TEST_ASSERT_EQUAL(20, iter1.size());
  TEST_ASSERT_EQUAL(30, iter2.size());
  TEST_ASSERT_EQUAL(sizeof(test_data) - 50, iter3.size());

  auto data1 = DataBuffer{iter1.begin(), iter1.end()};
  auto data2 = DataBuffer{iter2.begin(), iter2.end()};
  auto data3 = DataBuffer{iter3.begin(), iter3.end()};

  TEST_ASSERT_EQUAL_STRING_LEN(test_data, data1.data(), data1.size());
  TEST_ASSERT_EQUAL_STRING_LEN(
      test_data + static_cast<SafeStreamRingIndex::type>(iter2.begin_offset()),
      data2.data(), data2.size());
  TEST_ASSERT_EQUAL_STRING_LEN(
      test_data + static_cast<SafeStreamRingIndex::type>(iter3.begin_offset()),
      data3.data(), data3.size());
}

}  // namespace ae::test_send_data_action

int test_send_data_action() {
  UNITY_BEGIN();
  RUN_TEST(ae::test_send_data_action::test_SendingData);
  RUN_TEST(ae::test_send_data_action::test_SendingDataIter);
  return UNITY_END();
}
