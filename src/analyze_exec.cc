#include "bit_util.h"
#include "game.h"
#include "hadamard.h"
#include <iostream>
#include <stdexcept>
#include <limits>
#include <vector>
using std::cerr;
using std::cout;
using std::uint32_t;
using std::uint64_t;
using std::vector;
static_assert(std::numeric_limits<size_t>::digits >= 64, "you need a 64-bit computer");

constexpr uint64_t maxn = 1<<28; // 1GB of RAM

Game game;

void process_args(int argc, char **argv) {
  if (argc != 2) throw std::runtime_error("specify a game");
  game = Game(argv[1]);
}

uint32_t rare_position_bit;

vector<uint32_t> nimbers;
int nimber_bits = 0;

vector<uint64_t> rare;
bool rare_has_position;
uint32_t rare_mask;
// assuming position bit=0
vector<bool> rare_nimbers;
vector<bool> common_nimbers;

uint64_t largest_nimber_idx = 0;
vector<uint64_t> nimber_count(2, 0);  // including position bit

void recompute_rare() {
  uint64_t n = nimbers.size();
  vector<uint64_t> hadamard = nimber_count;
  hadamard_transform(hadamard.data(), hadamard.size());
  uint32_t best = 0;
  uint64_t best_val = (hadamard[0] + n)>>1;
  for(uint32_t i = 1; i < hadamard.size(); ++i) {
    if(rare_position_bit == 2 && (i&1u)==1u) continue;
    uint64_t val = (hadamard[i] + n) >> 1;
    if(val < best_val) { best = i; best_val = val; }
  }
  rare_mask = best >> 1;
  rare_has_position = (best & 1u);
  rare.clear();
  for (uint64_t i = 0; i < n; ++i) {
    bool is_rare = !parity(nimbers[i] & rare_mask);
    if (rare_has_position && (uint32_t(i&1u) ^ rare_position_bit)) {
      is_rare = !is_rare;
    }
    if (is_rare) rare.push_back(i);
  }
  if(rare.size() != best_val) throw std::runtime_error("hadamard gave wrong number of rare");

  rare_nimbers.assign(1u<<nimber_bits, false);
  common_nimbers.assign(1u<<nimber_bits, false);
  for(uint32_t i = 0; i < (1u << nimber_bits); ++i) {
    rare_nimbers[i] = !parity(i & rare_mask);
    common_nimbers[i] = !rare_nimbers[i];
  }
}

void print_stats() {
  const uint64_t n = nimbers.size();
  const uint64_t largest_rare_idx = rare.empty() ? 0 : rare.back();
  cout << "n=2^" << lowest_bit(n)
    << " largest G[" << largest_nimber_idx << "]=" << nimbers[largest_nimber_idx]
    << " rare_mask=" << std::hex << rare_mask << std::dec << (rare_has_position ? "pos" : "")
    << " num_rare=" << rare.size()
    << " last_rare=" << largest_rare_idx
    << std::endl;
}

bool check_period() {
  unsigned t = highest_bit(game.whole_moves() | game.take_moves() | game.split_moves());
  const uint64_t n = nimbers.size();
  for(uint64_t p = 1; p < n; ++p) {
    uint64_t start = n - p;
    while(start > 0u && nimbers[start-1u] == nimbers[start-1u+p]) {
      --start;
    }
    /*
     * To verify that the period persists, we need to check that, for start>0:
     * 1. n-p no longer has whole moves:
     *    n >= p + t + 1
     * 2. Take moves are same:
     *    G[n-p-t] = G[n-t]
     *    n-p-t >= max(start,1)
     *    n >= start + p + t
     * 3. Split moves are same:
     *    n-t = A+B, A>=B
     *    then A >= ceil((n-t)/2) must be >= st+p
     *    (n-t)/2 > st+p-1
     *    n-t >= 2*st+2*p-1
     *    n >= 2*start + 2*p + t - 1
     * All together:
     *    n >= max(2*start + 2*p + t - 1, p + t + 1)
     */
    if (game.equal_split_allowed() && n >= std::max(2*std::max(start,uint64_t(1)) + 2*p + t - 1, p + t + 1)) {
      cout << "Found period start=" << start << " period=" << p << "\n";
      return true;
    }
  }
  return false;
}

void compute_nimber() {
  const uint64_t n = nimbers.size();
  vector<bool> rare_remaining = rare_nimbers;
  vector<bool> common_remaining = common_nimbers;
  if (rare_has_position && ((n&1)^rare_position_bit)) {
    std::swap(rare_remaining, common_remaining);
  }

  // Process whole moves.
  if (n < 32 && get_bit(game.whole_moves(), n)) {
    rare_remaining[0] = false;
    common_remaining[0] = false;
  }

  // Process take moves.
  uint32_t take_moves = game.take_moves();
  while(take_moves) {
    uint32_t m = lowest_bit(take_moves);
    reset_bit(take_moves, m);
    if(m<n) {
      rare_remaining[nimbers[n-m]] = false;
      common_remaining[nimbers[n-m]] = false;
    }
  }

  // Process split moves rare+something.
  uint32_t split_moves = game.split_moves();
  while(split_moves) {
    uint32_t m = lowest_bit(split_moves);
    reset_bit(split_moves, m);
    for(uint64_t r : rare) {
      if (r==0) continue;
      if (r+m < n) {
        uint64_t r2 = n-r-m;
        if(!game.equal_split_allowed() && r==r2) continue;
        uint32_t v = nimbers[r] ^ nimbers[r2];
        rare_remaining[v] = false;
        common_remaining[v] = false;
      }
    }
  }

  // Find common candidate.
  uint32_t candidate = 0;
  while(candidate < common_remaining.size() && !common_remaining[candidate]) {
    ++candidate;
  }
  // Count rare remaining.
  uint32_t num_rare_remaining = 0;
  for(uint32_t i=0;i<candidate;++i) num_rare_remaining += rare_remaining[i];

  // Process all splits.
  split_moves = game.split_moves();
  while(split_moves) {
    uint32_t m = lowest_bit(split_moves);
    reset_bit(split_moves, m);
    for(uint64_t a = 1; a+m+a <= n; ++a) {
      if(num_rare_remaining == 0) break;
      uint64_t b = n-m-a;
      if(!game.equal_split_allowed() && a==b) continue;
      uint32_t v = nimbers[a] ^ nimbers[b];
      if (rare_remaining[v]) {
        rare_remaining[v] = false;
        if(v < candidate) {
          --num_rare_remaining;
        }
      }
    }
  }

  bool must_recompute_rare = false;
  if(num_rare_remaining) {
    candidate = 0;
    while(!rare_remaining[candidate]) ++candidate;
    must_recompute_rare = true;
  }
  if (candidate >= (1u << nimber_bits)) {
    ++nimber_bits;
    if(nimber_bits >= 30) throw std::runtime_error("nimbers too large");
    nimber_count.resize(1u << (nimber_bits+1));
    must_recompute_rare = true;
  }

  nimbers.push_back(candidate);
  ++nimber_count[(candidate << 1) | uint32_t((n^rare_position_bit)&1)];
  if(candidate > nimbers[largest_nimber_idx]) largest_nimber_idx = n;
  if (must_recompute_rare) recompute_rare();
}

void analyze_game() {
  if ((game.split_moves() & 0xaaaaaaaau)==0) rare_position_bit = 0;
  else if ((game.split_moves() & 0x55555555u)==0) rare_position_bit = 1;
  else rare_position_bit = 2;

  recompute_rare();

  for(;;) {
    if (is_power_of_2(nimbers.size())) {
      print_stats();
      if(check_period()) break;
    }
    if(nimbers.size() >= maxn) break;
    compute_nimber();
  }
}

int main(int argc, char **argv) {
  process_args(argc, argv);
  analyze_game();
}
