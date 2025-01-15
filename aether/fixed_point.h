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

#ifndef AETHER_FIXED_POINT_H_
#define AETHER_FIXED_POINT_H_

#include <limits>
#include <cstdint>
#include <algorithm>
#include <type_traits>

#include "aether/warning_disable.h"

DISABLE_WARNING_PUSH()
IGNORE_WNAN_INFINITY_DISABLED()
#include "third_party/gcem/include/gcem.hpp"
DISABLE_WARNING_POP()

namespace ae {
// Compile-time fixed point number representation.
// A number is stored as floating point value multiplied by scale.
// A point position meaning:
//   0 - range represented [0..1). For example, for std::uint8_t 1.0 would be
//   encoded as 256,
//       so the maximum value encoded is 1.0 - 1/256. Rounding is performed as
//       truncating. min value is 1/256
//   1 - a single bit represents the integer part of the number. range is
//   [0..2). min = 1/127 8 - float is just truncated to integer [0..255], min =
//   1 9 - floating point is multiplied by 0.5 and truncated. range = [0..510],
//   min = 2 -1 - range = [0..0.5), min value is 1/510

template <typename T, T max_value, int int_bits>
struct Fixed {
  using Type = T;
  static constexpr Type kMaxValue = max_value;
  static constexpr int kIntBits = int_bits;
  static constexpr int kTotalBits = sizeof(Type) * 8;

  static constexpr double kMinValue =
      kIntBits < kTotalBits
          ? 1.0 / gcem::pow(2.0, static_cast<double>(kTotalBits - kIntBits))
          : gcem::pow(2.0, static_cast<double>(kIntBits - kTotalBits));
  static constexpr double kMinValueInv = 1 / kMinValue;
  Type value_;

  constexpr Fixed() = default;

  constexpr explicit Fixed(Type value) : value_(value) {}

  template <typename ST, ST mv, int nb>
  constexpr Fixed(const Fixed<ST, mv, nb>& f) {
    value_ = Cast(f).value_;
  }

  constexpr Fixed(double value) : value_(Value(value)) {}

  constexpr operator double() const { return value_ * kMinValue; }

  auto& operator=(double value) {
    value_ = Value(value);
    return *this;
  }

  template <typename ST, ST mv, int nb>
  auto& operator=(const Fixed<ST, mv, nb>& s) {
    value_ = Cast(s).value_;
    return *this;
  }

  // TODO: force compile-time only.
  static constexpr Type Value(double f) {
    return static_cast<Type>(f * kMinValueInv);
  }

  template <typename ST, ST mv, int nb>
  constexpr Fixed<Type, kMaxValue, kIntBits>& Cast(const Fixed<ST, mv, nb>& f) {
    static_assert(kIntBits >= nb, "Assignment may cause overflow");

    using ThisType = Fixed<Type, kMaxValue, kIntBits>;
    return ThisType(f.value_);
  }
};

template <typename Type>
constexpr int CalculateIntegerBits(double v) {
  constexpr double rr = gcem::pow(2.0, (8.0 * sizeof(Type)));
  constexpr double c = 1.0 - 1.0 / rr;
  if (v > c) {
    return static_cast<int>(gcem::ceil(gcem::log2(v))) +
           (v > c * (1ul << static_cast<int>(gcem::ceil(gcem::log2(v)))) ? 1
                                                                         : 0);
  } else if (v >= c / 2) {
    return 0;
  } else {
    return -static_cast<int>(gcem::floor(gcem::log2(1 / v))) -
           (v > c / (1ul << static_cast<int>(gcem::floor(gcem::log2(1 / v))))
                ? 1
                : 0);
  }
}

template <typename T>
constexpr bool CompareDoubles(const double v1, const double v2) {
  return gcem::abs(v1 - v2) / gcem::max(v1, v2) <
         1.0 / gcem::pow(2.0, static_cast<double>(8 * sizeof(T)));
}

template <int n>
struct TypeSelector1 {
  using type = std::conditional_t<
      (n > 32), std::uint64_t,
      std::conditional_t<
          (n > 16), std::uint32_t,
          std::conditional_t<(n > 8), std::uint16_t, std::uint8_t>>>;
};

template <typename T, int n>
struct TypeSelector {
  using type =
      typename TypeSelector1<(n < static_cast<int>(sizeof(T) * 8)
                                  ? static_cast<int>(sizeof(T) * 8) - n
                                  : n - static_cast<int>(sizeof(T) * 8))>::type;
};

template <typename R, typename F1, typename F2>
constexpr auto Add(const F1& v1, const F2& v2) {
  constexpr double max = static_cast<double>(F1::kMaxValue) * F1::kMinValue +
                         static_cast<double>(F2::kMaxValue) * F2::kMinValue;
  constexpr auto ib = CalculateIntegerBits<R>(max);
  constexpr auto i = Fixed<R, 0, ib>::Value(max);
  double r = static_cast<double>(v1) + static_cast<double>(v2);
  using RT = Fixed<R, i, ib>;
  return RT(RT::Value(r));
}

template <typename T1, T1 max_value1, int ib1, typename T2, T2 max_value2,
          int ib2>
constexpr auto operator+(const Fixed<T1, max_value1, ib1>& v1,
                         const Fixed<T2, max_value2, ib2>& v2) {
  return Add<typename TypeSelector1<gcem::max(
      Fixed<T1, max_value1, ib1>::kTotalBits,
      Fixed<T2, max_value2, ib2>::kTotalBits)>::type>(v1, v2);
}

template <typename R, typename F1, typename F2>
constexpr auto Sub(const F1& v1, const F2& v2) {
  double r = static_cast<double>(v1) - static_cast<double>(v2);
  using RT = Fixed<R, F1::kMaxValue, F1::kIntBits>;
  return RT(RT::Value(r));
}

template <typename T1, T1 max_value1, int ib1, typename T2, T2 max_value2,
          int ib2>
constexpr auto operator-(const Fixed<T1, max_value1, ib1>& v1,
                         const Fixed<T2, max_value2, ib2>& v2) {
  return Sub<typename Fixed<T1, max_value1, ib1>::Type>(v1, v2);
}

template <typename R, typename F1, typename F2>
constexpr auto Mul(const F1& v1, const F2& v2) {
  constexpr double max = static_cast<double>(F1::kMaxValue) * F1::kMinValue *
                         static_cast<double>(F2::kMaxValue) * F2::kMinValue;
  constexpr auto ib = CalculateIntegerBits<R>(max);
  constexpr auto i = Fixed<R, 0, ib>::Value(max);
  double r = static_cast<double>(v1) * static_cast<double>(v2);
  using RT = Fixed<R, i, ib>;
  return RT(RT::Value(r));
}

template <typename T1, T1 max_value1, int ib1, typename T2, T2 max_value2,
          int ib2>
constexpr auto operator*(const Fixed<T1, max_value1, ib1>& v1,
                         const Fixed<T2, max_value2, ib2>& v2) {
  return Mul<typename TypeSelector1<gcem::max(
      Fixed<T1, max_value1, ib1>::kTotalBits,
      Fixed<T2, max_value2, ib2>::kTotalBits)>::type>(v1, v2);
}

template <typename R, R min_value, typename F1, typename F2>
constexpr auto Div(const F1& v1, const F2& v2) {
  constexpr double max = static_cast<double>(F1::kMaxValue) * F1::kMinValue /
                         static_cast<double>(min_value * F2::kMinValue);
  constexpr auto ib = CalculateIntegerBits<R>(max);
  constexpr auto i = Fixed<R, 0, ib>::Value(max);
  double r = static_cast<double>(v1) / static_cast<double>(v2);
  using RT = Fixed<R, i, ib>;
  return RT(RT::Value(r));
}

template <typename T1, T1 max_value1, int ib1, typename T2, T2 max_value2,
          int ib2>
constexpr auto operator/(const Fixed<T1, max_value1, ib1>& v1,
                         const Fixed<T2, max_value2, ib2>& v2) {
  return Div<typename TypeSelector1<gcem::max(
                 Fixed<T1, max_value1, ib1>::kTotalBits,
                 Fixed<T2, max_value2, ib2>::kTotalBits)>::type,
             Fixed<T2, max_value2, ib2>::Value(
                 Fixed<T2, max_value2, ib2>::kMinValue)>(v1, v2);
}

template <typename Exp, typename T, T max_value, int int_bits, T base, T scale>
struct Exponent : public Fixed<T, max_value, int_bits> {
  using Fix = Fixed<T, max_value, int_bits>;

  static constexpr auto kBase = base;
  static constexpr auto kScale = scale;

  // inherit all constructors from Fix
  using Fix::Fix;

  constexpr Exp Serialize() const {
    constexpr double lb = 1.0 / gcem::log2(kBase * Fix::kMinValue);
    constexpr double s = gcem::log2(1.0 / kScale) * lb;
    double p = gcem::log2(Fix::value_) * lb + s;
    return static_cast<Exp>(p);
  }

  void Deserialize(Exp stored) {
    constexpr double b = kBase * Fix::kMinValue;
    // convert stored to common type
    double e = gcem::pow(b, static_cast<std::uint64_t>(stored)) * kScale;
    Fix::value_ = static_cast<T>(e);
  }
};

// Options for an exponent type definition
template <typename T>
struct ExponentOptions {
  double base_;
  int total_bits_;
  int int_bits_;

  constexpr ExponentOptions(double min, double max)
      : base_{CalcBase(min, max)},
        total_bits_{
            static_cast<int>(gcem::ceil(gcem::log2(CalcRange(min, max))))},
        int_bits_{static_cast<int>(gcem::ceil(gcem::log2(max)))} {}

 private:
  static constexpr auto CalcBase(double min, double max) {
    double v_ = max / min;
    double n_ = gcem::pow(2ul, sizeof(T) * 8) - 1;
    return gcem::pow(v_, 1 / n_);
  }

  static constexpr auto CalcRange(double min, double max) {
    double v_ = max / min;
    double n_ = gcem::pow(2ul, sizeof(T) * 8) - 1;
    double base = gcem::pow(v_, 1 / n_);
    double step_ = min * base - min;
    return v_ / step_;
  }
};

template <typename T>
constexpr ExponentOptions<T> make_exponent_options(double min, double max) {
  return ExponentOptions<T>(min, max);
}
}  // namespace ae

namespace std {
template <typename T, T max_value, int position>
class numeric_limits<ae::Fixed<T, max_value, position>> {
 public:
  static constexpr double lowest() { return 0.0; }
  static constexpr double min() {
    return ae::Fixed<T, max_value, position>::kMinValue;
  }
  static constexpr double max() {
    return ae::Fixed<T, max_value, position>::kMaxValue;
  }
};
}  // namespace std

#define AE_FIXED(T, M)                                                \
  Fixed<T, ae::Fixed<T, 0, ae::CalculateIntegerBits<T>(M)>::Value(M), \
        ae::CalculateIntegerBits<T>(M)>

#define _EXP_OPT_(T, MIN, MAX) ae::make_exponent_options<T>(MIN, MAX)

#define _FIXED_OPT_(T, MIN, MAX) \
  AE_FIXED(ae::TypeSelector1<_EXP_OPT_(T, MIN, MAX).total_bits_>::type, MAX)

#define AE_EXPONENT(T, MIN, MAX)                                              \
  ae::Exponent<T, _FIXED_OPT_(T, MIN, MAX)::Type,                             \
               _FIXED_OPT_(T, MIN, MAX)::kMaxValue,                           \
               _EXP_OPT_(T, MIN, MAX).int_bits_,                              \
               _FIXED_OPT_(T, MIN, MAX)::Value(_EXP_OPT_(T, MIN, MAX).base_), \
               _FIXED_OPT_(T, MIN, MAX)::Value(MIN)>

#endif  // AETHER_FIXED_POINT_H_ */
