#include "simple_solver.h"
#include <limits>
#include <stdexcept>

std::uint32_t mex(const std::vector<std::uint32_t> &v) {
  if (v.size() >= std::numeric_limits<std::uint32_t>::max()) {
    throw std::domain_error("too many mex params");
  }
  std::vector<bool> seen(v.size() + 1u, false);
  for (auto x : v) {
    if (x < seen.size()) seen[x] = true;
  }
  size_t res = 0;
  while (seen[res]) ++res;
  return res;
}
