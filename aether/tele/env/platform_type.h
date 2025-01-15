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

#ifndef AETHER_TELE_ENV_PLATFORM_TYPE_H_
#define AETHER_TELE_ENV_PLATFORM_TYPE_H_

#if !defined AE_PLATFORM_TYPE
#  if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#    define AE_PLATFORM_TYPE "Windows"
#  elif defined(__linux__) || defined(__gnu_linux__)
#    define AE_PLATFORM_TYPE "Linux"
#  elif defined(__FreeBSD__)
#    define AE_PLATFORM_TYPE "FreeBSD"
#  elif defined __APPLE__
#    include <TargetConditionals.h>
#    if TARGET_OS_IPHONE
#      define AE_PLATFORM_TYPE "IOS"
#    elif TARGET_OS_MAC
#      define AE_PLATFORM_TYPE "macOs"
#    endif
#  elif defined __ANDROID__
#    define AE_PLATFORM_TYPE "Android"
#  elif defined ESP_PLATFORM
#    include <sdkconfig.h>
#    define AE_PLATFORM_TYPE CONFIG_IDF_TARGET
#  else
#    warning "Unknown platform"
#    warning \
        "Please define AE_PLATFORM_TYPE to name your platform in statistics"
#    define AE_PLATFORM_TYPE "Unknown"
#  endif
#endif

#endif  // AETHER_TELE_ENV_PLATFORM_TYPE_H_ */
