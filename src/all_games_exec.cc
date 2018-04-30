#include "bit_util.h"
#include "game.h"
#include "game_util.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <vector>

unsigned num_nimbers;

void process_args(int argc, char **argv) {
  num_nimbers = 0;
  int i=1;
  while(i < argc) {
    std::string a = argv[i++];
    if (a == "-n") {
      if (i >= argc) throw std::runtime_error("missing arg");
      unsigned long x = std::stoul(argv[i++]);
      if (x > std::numeric_limits<unsigned>::max() / 4) {
        throw std::runtime_error("invalid limit");
      }
      num_nimbers = x;
    } else {
      throw std::runtime_error("unknown parameter " + a);
    }
  }

  if (num_nimbers == 0) {
    throw std::runtime_error("invalid limit");
  }
}

std::vector<Game> generate_octal_games() {
  std::vector<Game> octal_games;

  for (char c0 : {'0', '4'}) {
    for (char c1 = '0'; c1 <= '7'; ++c1) {
      for (char c2 = '0'; c2 <= '7'; ++c2) {
        for (char c3 = '0'; c3 <= '7'; ++c3) {
          Game game(std::string() + c0 + "." + c1 + c2 + c3);
          // Skip the zero game.
          if (game == Game()) continue;
          octal_games.push_back(game);
        }
      }
    }
  }

  return octal_games;
}

Game normalize_game(const Game &game) {
  if (!game.equal_split_allowed()) {
    throw std::runtime_error("can't normalize grundy");
  }
  std::uint32_t whole_moves = game.whole_moves();
  std::uint32_t take_moves = game.take_moves();
  std::uint32_t split_moves = game.split_moves();
  // While can't take whole 1, reduce heap size.
  while (!get_bit(whole_moves, 1)) {
    if (get_bit(split_moves, 31)) {
      throw std::runtime_error("too large split moves");
    }
    split_moves <<= 1;
    take_moves |= split_moves;
    whole_moves = (whole_moves>>1) | take_moves;
  }
  const auto nimbers = brute_force_nimbers(Game(whole_moves, take_moves, split_moves), 32);
  /*
   * If G[i]!=0, might as well allow taking whole i.
   */
  for (int i=1;i<=3;++i) {
    if (nimbers[i] != 0) {
      set_bit(whole_moves, i);
    }
  }
  /*
   * If G[i]==0 and can split n=i+(n-i), then might as well allow taking i.
   */
  for (int i=2;i<=3;++i) {
    if (nimbers[i] == 0 && get_bit(split_moves, 0)) {
      set_bit(take_moves, i);
    }
  }
  /*
   * If G[2]==0 and can split n=2+(n-3), then might as well allow taking 3.
   */
  if (nimbers[2] == 0 && get_bit(split_moves, 1)) {
    set_bit(take_moves, 3);
  }

  return Game(whole_moves, take_moves, split_moves);
}

struct GameType {
  bool solved = false;
  // only if solved:
  unsigned period_start = 0;
  unsigned period = 0;
  std::vector<std::uint32_t> nimbers;
  // only if not solved:
  Game normalized_game;
};

struct EquivalentGames {
  std::vector<Game> games;
  Game representative;
};

bool operator<(const GameType &a, const GameType &b) {
  if (a.solved != b.solved) return a.solved;
  if (a.solved) {
    if (a.period_start != b.period_start) {
      return a.period_start < b.period_start;
    }
    if (a.period != b.period) {
      return a.period < b.period;
    }
    return a.nimbers < b.nimbers;
  } else {
    return a.normalized_game.name() < b.normalized_game.name();
  }
}

void try_to_solve(const Game &game, GameType &game_type) {
  if (!game.equal_split_allowed()) {
    throw std::runtime_error("can't solve grundy's game");
  }

  const std::vector<std::uint32_t> nimbers =
    brute_force_nimbers(game, num_nimbers);

  unsigned t = highest_bit(game.whole_moves() | game.take_moves() | game.split_moves());

  for (unsigned period = 1; period < num_nimbers; ++period) {
    unsigned start = num_nimbers - period;
    while(start > 0u && nimbers[start-1u] == nimbers[start-1u + period]) {
      --start;
    }

    /*
     * To verify that the period persists, we need to check that:
     * 1. limit-p no longer has whole moves:
     *    limit >= p + t + 1
     * 2. Take moves are same:
     *    G[limit-p-t] = G[limit-t]
     *    limit-p-t >= start
     *    limit >= start + p + t
     * 3. Split moves are same:
     *    limit-t = A+B, A>=B
     *    then A >= ceil((limit-t)/2) must be >= st+p
     *    (limit-t)/2 > st+p-1
     *    limit-t >= 2*st+2*p-1
     *    limit >= 2*start + 2*p + t - 1
     * All together:
     *    limit >= max(2*start + 2*p + t - 1, p + t + 1)
     */
    if (num_nimbers >= std::max(2*start + 2*period + t - 1, period + t + 1)) {
      game_type.solved = true;
      game_type.period_start = start;
      game_type.period = period;
      game_type.nimbers.assign(nimbers.begin(), nimbers.begin() + start + period);
      break;
    }
  }
  if (!game_type.solved) {
    game_type.normalized_game = game;
  }
}

bool simpler_game(const Game &a, const Game &b) {
  const int at = highest_bit(a.whole_moves() | a.take_moves() | a.split_moves());
  const int bt = highest_bit(b.whole_moves() | b.take_moves() | b.split_moves());
  if (at != bt) return at < bt;
  if (a.split_moves() != b.split_moves()) return a.split_moves() < b.split_moves();
  if (a.take_moves() != b.take_moves()) return a.take_moves() < b.take_moves();
  if (a.whole_moves() != b.whole_moves()) return a.whole_moves() < b.whole_moves();
  return false;
}

std::vector<std::uint32_t> compute_reduced_nimbers(const Game &game) {
  std::vector<std::uint32_t> nimbers = brute_force_nimbers(game, num_nimbers + 10u);
  unsigned st = 0;
  while(st<10u && nimbers[st]==0u) ++st;
  if (nimbers[st] == 0u) throw std::runtime_error("too many zeros");
  return std::vector<std::uint32_t>(nimbers.begin() + st, nimbers.begin() + st + num_nimbers);
}

void find_representatives(std::map<GameType, EquivalentGames> &game_map) {
  for (auto &game_map_entry : game_map) {
    EquivalentGames &eq = game_map_entry.second;
    eq.representative = *std::min_element(eq.games.begin(), eq.games.end(), simpler_game);
  }
}
void verify_game_map(const std::map<GameType, EquivalentGames> &game_map) {
  std::map<std::vector<std::uint32_t>, Game> reverse_map;

  // Check all games within group seem same, all games in different groups seem different.
  for (const auto &game_map_entry : game_map) {
    const EquivalentGames &eq = game_map_entry.second;

    const std::vector<std::uint32_t> reduced_nimbers =
      compute_reduced_nimbers(eq.games[0]);

    for (size_t i = 1; i < eq.games.size(); ++i) {
      if (reduced_nimbers != compute_reduced_nimbers(eq.games[i])) {
        throw std::runtime_error("Different games in the same group: " + eq.games[0].name() + " " +
              eq.games[i].name());
      }
    }

    auto it = reverse_map.find(reduced_nimbers);
    if (it != reverse_map.end()) {
      throw std::runtime_error(
          "Different groups seem same: " + it->second.name() + " " + eq.representative.name());
    } else {
      reverse_map[reduced_nimbers] = eq.representative;
    }
  }

  // Check that the Grundy's game seems different that regular games.
  {
    const std::vector<std::uint32_t> reduced_nimbers =
      compute_reduced_nimbers(Game::grundy());

    auto it = reverse_map.find(reduced_nimbers);
    if (it != reverse_map.end()) {
      throw std::runtime_error(
        "Grundy's game seems same as " + it->second.name());
    }
  }
}


template<typename K, typename V>
const V &map_find(const std::map<K, V> &m, const K &key) {
  auto it = m.find(key);
  if (it == m.end()) throw std::runtime_error("key not found in map");
  return it->second;
}

void print_equivalence_tables(const std::map<std::string, GameType> &game_types,
                              const std::map<GameType, EquivalentGames> &game_map) {
  std::cout << "Equivalence tables.\n";

  std::cout << "\n";
  std::cout << "game | 1    | 2    | 3    | 4    | 5    | 6    | 7\n";
  std::cout << ":--- | :--- | :--- | :--- | :--- | :--- | :--- | :---\n";
  for (char c0 : {'0', '4'}) {
    std::cout << c0 << ".x";
    for (char c1 = '1'; c1 <= '7'; ++c1) {
      const Game game(std::string() + c0 + "." + c1);
      const Game repr = map_find(game_map, map_find(game_types, game.name())).representative;
      std::cout << "  | " << std::left << std::setw(3);
      if (repr == game) {
        std::cout << "-";
      } else {
        std::cout << repr.name();
      }
    }
    std::cout << "\n";
  }

  std::cout << "\n";
  std::cout << "game  | 1    | 2    | 3    | 4    | 5    | 6    | 7\n";
  std::cout << ":---  | :--- | :--- | :--- | :--- | :--- | :--- | :---\n";
  for (char c0 : {'0', '4'}) {
    for (char c1 = '0'; c1 <= '7'; ++c1) {
      std::cout << c0 << "." << c1 << "x ";
      for (char c2 = '1'; c2 <= '7'; ++c2) {
        const Game game(std::string() + c0 + "." + c1 + c2);
        const Game repr = map_find(game_map, map_find(game_types, game.name())).representative;
        std::cout << " | " << std::left << std::setw(4);
        if (repr == game) {
          std::cout << "-";
        } else {
          std::cout << repr.name();
        }
      }
      std::cout << "\n";
    }
  }

  std::cout << "\n";
  std::cout << "game   | 1     | 2     | 3     | 4     | 5     | 6     | 7\n";
  std::cout << ":----  | :---- | :---- | :---- | :---- | :---- | :---- | :----\n";
  for (char c0 : {'0', '4'}) {
    for (char c1 = '0'; c1 <= '7'; ++c1) {
      for (char c2 = '0'; c2 <= '7'; ++c2) {
        std::cout << c0 << "." << c1 << c2 << "x ";
        for (char c3 = '1'; c3 <= '7'; ++c3) {
          const Game game(std::string() + c0 + "." + c1 + c2 + c3);
          const Game repr = map_find(game_map, map_find(game_types, game.name())).representative;
          std::cout << " | " << std::left << std::setw(5);
          if (repr == game) {
            std::cout << "-";
          } else {
            std::cout << repr.name();
          }
        }
        std::cout << "\n";
      }
    }
  }
}

void print_games(const std::map<GameType, EquivalentGames> &game_map) {
  // Sort game map, shorter names first.
  std::vector<std::pair<Game, const GameType*>> games_by_name;
  for (const auto &p : game_map) {
    games_by_name.push_back({p.second.representative, &p.first});
  }
  std::sort(games_by_name.begin(),
            games_by_name.end(),
            [](const std::pair<Game, const GameType*> &a,
               const std::pair<Game, const GameType*> &b) {
              const std::string a_name = a.first.name();
              const std::string b_name = b.first.name();
              if (a_name.size() != b_name.size()) return a_name.size() < b_name.size();
              return a_name < b_name;
            });

  // Print solved games.
  std::cout << "\nTrivial games:\n\n";
  std::cout << "game  | prefix | period\n";
  std::cout << ":---- | -----: | -----:\n";
  for (const auto &p : games_by_name) {
    if (p.second->solved) {
      std::cout << std::setw(5) << std::left << p.first.name() << " | "
                << std::setw(6) << std::right << p.second->period_start << " | "
                << std::setw(6) << std::right << p.second->period << "\n";
    }
  }

  // Print unsolved games.
  std::cout << "\nNontrial games:\n";
  unsigned count_unsolved = 0;
  for (const auto &p : games_by_name) {
    if (!p.second->solved) {
      std::cout << p.first.name() << "\n";
      ++count_unsolved;
    }
  }
}

void process_all_games() {
  std::map<std::string, GameType> game_types;
  std::map<GameType, EquivalentGames> game_map;

  for (const Game &game : generate_octal_games()) {
    GameType game_type;
    const Game normalized_game = normalize_game(game);
    try_to_solve(normalized_game, game_type);
    game_types[game.name()] = game_type;
    game_map[game_type].games.push_back(game);
  }

  find_representatives(game_map);
  verify_game_map(game_map);
  print_equivalence_tables(game_types, game_map);
  print_games(game_map);
}

int main(int argc, char **argv) {
  process_args(argc, argv);
  process_all_games();
}
