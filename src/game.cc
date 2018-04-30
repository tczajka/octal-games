#include "bit_util.h"
#include "game.h"
#include <stdexcept>
using std::string;
using std::uint32_t;

Game::Game(const string &name) {
  if (name == "grundy") {
    *this = grundy();
  } else {
    string name2 = name;
    if(name2.size() >= 1u && name2[0]=='.') name2 = "0" + name2;
    if(name2.size() >= 2u) {
      if (name2[1] != '.') throw std::domain_error("Invalid name " + name);
      name2.erase(1, 1);
    }
    if(name2.size() > 32) throw std::domain_error("Game name too long " + name);
    for (unsigned i = 0; i < name2.size(); ++i) {
      if (name2[i] < '0' || name2[i] > '7') throw std::domain_error("Invalid game name " + name);
      unsigned c = name2[i] - '0';
      if (get_bit(c,0)) set_bit(m_whole_moves, i);
      if (c & 2u) set_bit(m_take_moves, i);
      if (c & 4u) set_bit(m_split_moves, i);
    }
    if (m_whole_moves & 1u) throw std::domain_error("Infinite game");
    if (m_take_moves & 1u) throw std::domain_error("Infinite game");
  }
}

Game::Game(const uint32_t _whole_moves,
           const uint32_t _take_moves,
           const uint32_t _split_moves) :
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

string Game::name() const {
  if (!m_equal_split_allowed) {
    if (*this == grundy()) {
      return "grundy";
    } else {
      throw std::domain_error("Can't name this game");
    }
  } else {
    string res;
    for (unsigned t = 0; t < 32; ++t) {
      unsigned c = 0;
      if (get_bit(m_whole_moves, t)) set_bit(c, 0);
      if (get_bit(m_take_moves, t)) set_bit(c, 1);
      if (get_bit(m_split_moves, t)) set_bit(c, 2);
      res += static_cast<char>('0' + c);
    }
    while(res.size() > 1 && res.back() == '0') res.pop_back();
    if (res.size() > 1) {
      res = (res[0]=='0' ? string() : res.substr(0, 1)) + "." + res.substr(1);
    }
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
