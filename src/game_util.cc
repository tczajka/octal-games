#include "game_util.h"

#include "bit_util.h"
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

std::vector<std::uint32_t> brute_force_nimbers(const Game &game, const size_t size) {
  if (size > std::numeric_limits<size_t>::max() / 4) {
    throw std::domain_error("size too large");
  }

  std::vector<std::uint32_t> nimbers(size, 0);

  for (size_t n = 0; n < size; ++n) {
    std::vector<std::uint32_t> excluded;

    if (n < 32u && get_bit(game.whole_moves(), n)) {
      excluded.push_back(0);
    }

    auto take_moves = game.take_moves();
    while (take_moves) {
      const auto m = lowest_bit(take_moves);
      reset_bit(take_moves, m);

      if(m + 1u <= n) excluded.push_back(nimbers[n-m]);
    }

    auto split_moves = game.split_moves();
    while (split_moves) {
      const auto m = lowest_bit(split_moves);
      reset_bit(split_moves, m);

      for (size_t a = 1; a+a+m <= n; ++a) {
        const size_t b = n - m - a;
        if (!game.equal_split_allowed() && a==b) continue;
        excluded.push_back(nimbers[a] ^ nimbers[b]);
      }
    }

    nimbers[n] = mex(excluded);
  }

  return nimbers;
}
