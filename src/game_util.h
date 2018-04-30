#ifndef GAME_UTIL_H
#define GAME_UTIL_H

#include "game.h"
#include <cstdint>
#include <vector>

std::uint32_t mex(const std::vector<std::uint32_t> &v);

std::vector<std::uint32_t> brute_force_nimbers(const Game &game, size_t size);

std::string nimbers_to_string(const std::vector<std::uint32_t> &);

#endif
