#pragma once

#include "stdint.h"

#ifdef __clang__
#define maybe_unused __attribute__((used))
#else
#define maybe_unused
#endif

maybe_unused static inline bool is_zero(float x) {
  return x == 0.f || x == -0.f;
}

maybe_unused static inline bool is_zero(double x) {
  return x == 0. || x == -0.;
}

template <typename T> static inline T min(T x, T y) { return (x < y) ? x : y; }

template <typename T> static inline T max(T x, T y) { return (x > y) ? x : y; }

template <typename T> static inline T clamp(T x, T lower_bound, T upper_bound) {
  x = min(x, upper_bound);
  x = max(x, lower_bound);
  return x;
}

template <typename T> static inline T abs(T x) {
  if (x >= 0)
    return x;
  else
    return -x;
}