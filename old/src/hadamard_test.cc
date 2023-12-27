#include "hadamard.h"
#include "bit_util.h"
#include "gtest/gtest.h"
#include <random>
using std::uint32_t;
using std::uint64_t;

TEST(HadamardTest, test_hadamard_transform) {
  uint64_t a[64];
  uint64_t b[64];

  std::mt19937 rng;
  std::uniform_int_distribution<uint64_t> dist;
  for (int i=0;i<64;++i) a[i]=b[i]=dist(rng);

  hadamard_transform(b, 64);

  for (uint32_t i=0;i<64;++i) {
    uint64_t res = 0;
    for (uint32_t j=0;j<64;++j) {
      if(parity(i&j)) res -= a[j]; else res += a[j];
    }
    ASSERT_EQ(b[i], res);
  }
}
