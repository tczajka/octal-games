#include "bit_util.h"
#include "game.h"
#include <stdexcept>

Game::Game(const std::string &name) {
  if (name == "grundy") {
    *this = grundy();
  } else {
    if(name.size() > 33) throw std::domain_error("Game name too long " + name);
    for (unsigned i = 0; i < name.size(); ++i) {
      if (i==1u) {
        if (name[i] != '.') throw std::domain_error("Invalid game name " + name);
      } else {
        unsigned m = i==0 ? 0 : i-1u;
        if (name[i] < '0' || name[i] > '7') throw std::domain_error("Invalid game name " + name);
        unsigned c = name[i] - '0';
        if (get_bit(c,0)) set_bit(m_whole_moves, m);
        if (c & 2u) set_bit(m_take_moves, m);
        if (c & 4u) set_bit(m_split_moves, m);
      }
    }
    if (m_whole_moves & 1u) throw std::domain_error("Infinite game");
    if (m_take_moves & 1u) throw std::domain_error("Infinite game");
  }
}

Game::Game(const std::uint32_t _whole_moves,
           const std::uint32_t _take_moves,
           const std::uint32_t _split_moves) :
    m_whole_moves{_whole_moves},
    m_take_moves{_take_moves},
    m_split_moves{_split_moves} {
  if (m_whole_moves & 1u) throw std::domain_error("Infinite game");
  if (m_take_moves & 1u) throw std::domain_error("Infinite game");
}

Game Game::grundy() {
  Game g(0, 0, 1);
  g.m_equal_split_allowed = false;
  return g;
}

std::string Game::name() const {
  if (!m_equal_split_allowed) {
    if (*this == grundy()) {
      return "grundy";
    } else {
      throw std::domain_error("Can't name this game");
    }
  } else {
    std::string res;
    for (unsigned t = 0; t < 32; ++t) {
      unsigned c = 0;
      if (get_bit(m_whole_moves, t)) set_bit(c, 0);
      if (get_bit(m_take_moves, t)) set_bit(c, 1);
      if (get_bit(m_split_moves, t)) set_bit(c, 2);
      res += static_cast<char>('0' + c);
    }
    while(res.size() > 1 && res.back() == '0') res.pop_back();
    if (res.size() > 1) res = res.substr(0, 1) + "." + res.substr(1);
    return res;
  }
}

bool Game::operator==(const Game &other) const {
  return
    m_whole_moves == other.m_whole_moves &&
    m_take_moves == other.m_take_moves &&
    m_split_moves == other.m_split_moves &&
    m_equal_split_allowed == other.m_equal_split_allowed;
}

bool Game::operator!=(const Game &other) const {
  return !(*this == other);
}
