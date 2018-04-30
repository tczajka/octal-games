#ifndef BIT_UTIL_H
#define BIT_UTIL_H

#include <cstdint>
#include <limits>

inline unsigned lowest_bit(std::uint32_t x) {
  static_assert(std::numeric_limits<unsigned>::digits == 32, "unsigned != uint32_t");
  return __builtin_ctz(x);
}

inline unsigned lowest_bit(std::uint64_t x) {
  static_assert(std::numeric_limits<unsigned long long>::digits == 64,
                "unsigned long long != uint64_t");

  return __builtin_ctzll(x);
}

#endif
