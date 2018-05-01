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

inline unsigned highest_bit(std::uint32_t x) {
  static_assert(std::numeric_limits<unsigned>::digits == 32, "unsigned != uint32_t");
  return 31 - __builtin_clz(x);
}

inline unsigned highest_bit(std::uint64_t x) {
  static_assert(std::numeric_limits<unsigned long long>::digits == 64,
                "unsigned long long != uint64_t");

  return 63 - __builtin_clzll(x);
}

inline bool parity(std::uint32_t x) {
  static_assert(std::numeric_limits<unsigned>::digits == 32, "unsigned != uint32_t");
  return __builtin_parity(x);
}

inline bool parity(std::uint64_t x) {
  static_assert(std::numeric_limits<unsigned long long>::digits == 64,
                "unsigned long long != uint64_t");
  return __builtin_parityll(x);
}

template<typename T>
inline bool get_bit(T x, unsigned bit) {
  return x & (T(1) << bit);
}

template<typename T>
inline void set_bit(T &x, unsigned bit) {
  x |= T(1) << bit;
}

template<typename T>
inline void reset_bit(T &x, unsigned bit) {
  x &= ~(T(1) << bit);
}

template<typename T>
inline bool is_power_of_2(T x) {
  return x > T(0) && (x & (x-T(1))) == T(0);
}

#endif
