#include "hadamard.h"
#include "bit_util.h"
#include <stdexcept>
using std::size_t;

namespace {
  void hadamard_transform_rec(std::uint64_t *const data, const size_t n) {
    if (n == 1u) return;
    const size_t n2 = n/2u;

    hadamard_transform_rec(data, n2);
    hadamard_transform_rec(data + n2, n2);
    for (size_t i = 0; i < n2; ++i) {
      const uint64_t A = data[i];
      const uint64_t B = data[i + n2];
      data[i] = A + B;
      data[i + n2] = A - B;
    }
  }
}

void hadamard_transform(std::uint64_t *const data, const size_t n) {
  if (!is_power_of_2(n)) throw std::runtime_error("n must be power of 2");

  hadamard_transform_rec(data, n);
}
