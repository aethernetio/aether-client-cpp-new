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

#ifndef AETHER_TELE_ENV_LIBRARY_VERSION_H_
#define AETHER_TELE_ENV_LIBRARY_VERSION_H_

#if defined AE_GIT_VERSION
#  define LIBRARY_VERSION AE_PROJECT_VERSION "." AE_GIT_VERSION
#elif defined AE_PROJECT_VERSION
#  define LIBRARY_VERSION AE_PROJECT_VERSION
#else
#  define LIBRARY_VERSION "0.0.0"
#  if defined _MSC_VER
#    pragma message("Library version is not defined.")
#  else
#    warning "Library version is not defined."
#  endif
#endif

#endif  // AETHER_TELE_ENV_LIBRARY_VERSION_H_ */
