#ifndef GAME_H
#define GAME_H

#include <cstdint>
#include <string>

class Game {
public:
  explicit Game() : Game(0, 0, 0) {}
  /*
   * E.g. Game(".123") or Game("grundy").
   */
  explicit Game(const std::string &name);

  explicit Game(const std::uint32_t _whole_moves,
                const std::uint32_t _take_moves,
                const std::uint32_t _split_moves);

  static Game grundy();

  std::string name() const;

  std::uint32_t whole_moves() const { return m_whole_moves; }
  std::uint32_t take_moves() const { return m_take_moves; }
  std::uint32_t split_moves() const { return m_split_moves; }
  bool equal_split_allowed() const { return m_equal_split_allowed; }

  bool operator==(const Game &other) const;
  bool operator!=(const Game &other) const;

private:
  std::uint32_t m_whole_moves = 0;
  std::uint32_t m_take_moves = 0;
  std::uint32_t m_split_moves = 0;
  bool m_equal_split_allowed = true;
};

#endif
