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

#ifndef AETHER_TELE_DEFINES_H_
#define AETHER_TELE_DEFINES_H_

#include "aether/tele/collectors.h"
#include "aether/tele/env_collectors.h"
#include "aether/tele/register.h"

#ifndef TAG_LIST_NAME
#  error "definition of TAG_LIST_NAME is required to use ae::tele"
#endif

#ifndef TELE_SINK
#  error "definition of TELE_SINK is required to use ae::tele"
#endif

#define AE_CAT_(a, b) a##b
#define AE_CAT(a, b) AE_CAT_(a, b)
#define AE_UNIQUE_NAME(P) AE_CAT(P, AE_CAT(__LINE__, __COUNTER__))

#define AE_TELE_DIAGNOSTIC_(TAG_LIST, LITERAL)          \
  static_assert((AE_TAG_INDEX(TAG_LIST, LITERAL) >= 0), \
                #LITERAL " is not registered");

#define AE_TELE_(LITERAL, LEVEL, ...)                                        \
  AE_TELE_DIAGNOSTIC_(TAG_LIST_NAME, LITERAL)                                \
  [[maybe_unused]] ae::tele::Tele<TELE_SINK, LEVEL,                          \
                                  AE_TAG_MODULE(TAG_LIST_NAME, LITERAL)>     \
  AE_UNIQUE_NAME(TELE_) {                                                    \
    TELE_SINK::Instance(), AE_TAG_INDEX(TAG_LIST_NAME, LITERAL), __VA_ARGS__ \
  }

#define AE_TELE_DEBUG(LITERAL, ...)                                       \
  AE_TELE_(LITERAL, ae::tele::Level::kDebug, __FILE__, __LINE__, LITERAL, \
           __VA_ARGS__)
#define AE_TELE_INFO(LITERAL, ...)                                       \
  AE_TELE_(LITERAL, ae::tele::Level::kInfo, __FILE__, __LINE__, LITERAL, \
           __VA_ARGS__)
#define AE_TELE_WARNING(LITERAL, ...)                                       \
  AE_TELE_(LITERAL, ae::tele::Level::kWarning, __FILE__, __LINE__, LITERAL, \
           __VA_ARGS__)
#define AE_TELE_ERROR(LITERAL, ...)                                       \
  AE_TELE_(LITERAL, ae::tele::Level::kError, __FILE__, __LINE__, LITERAL, \
           __VA_ARGS__)

// For simple logging
#define AE_TELED_DEBUG(...)                                           \
  AE_TELE_("LOG", ae::tele::Level::kDebug, __FILE__, __LINE__, "LOG", \
           __VA_ARGS__)
#define AE_TELED_INFO(...)                                           \
  AE_TELE_("LOG", ae::tele::Level::kInfo, __FILE__, __LINE__, "LOG", \
           __VA_ARGS__)
#define AE_TELED_WARNING(...)                                           \
  AE_TELE_("LOG", ae::tele::Level::kWarning, __FILE__, __LINE__, "LOG", \
           __VA_ARGS__)
#define AE_TELED_ERROR(...)                                           \
  AE_TELE_("LOG", ae::tele::Level::kError, __FILE__, __LINE__, "LOG", \
           __VA_ARGS__)

// Log environment data
#define AE_TELE_ENV(...)                                   \
  ae::tele::EnvTele<TELE_SINK> AE_UNIQUE_NAME(TELE_ENV_) { \
    TELE_SINK::Instance() __VA_ARGS__                      \
  }

#endif  // AETHER_TELE_DEFINES_H_ */
