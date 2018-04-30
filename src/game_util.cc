#include "game_util.h"

#include "bit_util.h"
#include <limits>
#include <stdexcept>

using std::string;
using std::vector;
using std::uint32_t;

uint32_t mex(const vector<uint32_t> &v) {
  if (v.size() >= std::numeric_limits<uint32_t>::max()) {
    throw std::domain_error("too many mex params");
  }
  vector<bool> seen(v.size() + 1u, false);
  for (auto x : v) {
    if (x < seen.size()) seen[x] = true;
  }
  size_t res = 0;
  while (seen[res]) ++res;
  return res;
}

vector<uint32_t> brute_force_nimbers(const Game &game, const size_t size) {
  if (size > std::numeric_limits<size_t>::max() / 4) {
    throw std::domain_error("size too large");
  }

  vector<uint32_t> nimbers(size, 0);

  for (size_t n = 0; n < size; ++n) {
    vector<uint32_t> excluded;

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

string nimbers_to_string(const vector<uint32_t> &nimbers) {
  string res(nimbers.size(), '?');
  for (size_t i = 0u; i < nimbers.size(); ++i) {
    const auto n = nimbers[i];
    if (n < 10u) res[i] = static_cast<char>('0' + n);
    else if (n < 36u) res[i] = static_cast<char>('A' + (n - 10));
  }
  return res;
}
