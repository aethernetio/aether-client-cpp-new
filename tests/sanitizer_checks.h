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

#ifndef TESTS_SANITIZER_CHECKS_H_
#define TESTS_SANITIZER_CHECKS_H_

#if defined __clang__
#  define TEST_ASAN_ENABLED __has_feature(address_sanitizer)
#elif defined __GNUC__
#  define TEST_ASAN_ENABLED defined __SANITIZE_ADDRESS__
#else
#  define TEST_ASAN_ENABLED
#endif

#endif  // TESTS_SANITIZER_CHECKS_H_
