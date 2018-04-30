#include "game_util.h"
#include "gtest/gtest.h"

TEST(GameUtilTest, test_mex) {
  EXPECT_EQ(mex({}), 0u);
  EXPECT_EQ(mex({2,1,2}), 0u);
  EXPECT_EQ(mex({0,3,0,1}), 2u);
  EXPECT_EQ(mex({0,3,1,0xffffffffu}), 2u);
}

TEST(GameUtilTest, test_brute_force_numbers) {
  // Zero game.
  EXPECT_EQ(brute_force_nimbers(Game(0, 0, 0), 10),
            std::vector<std::uint32_t>(10, 0));

  // Game 0.4, based on Winning Ways.
  EXPECT_EQ(brute_force_nimbers(Game("0.4"), 20),
            (std::vector<std::uint32_t>{0, 0, 0, 1, 1, 2, 0, 3, 1, 1,
                                        0, 3, 3, 2, 2, 4, 0, 5, 2, 2}));
  // Game 0.124, based on Winning Ways.
  EXPECT_EQ(brute_force_nimbers(Game("0.124"), 20),
            (std::vector<std::uint32_t>{0, 1, 0, 0, 1, 1, 0, 2, 1, 3,
                                        0, 2, 1, 3, 0, 1, 1, 3, 0, 2}));
  // Grundy's game.
  EXPECT_EQ(brute_force_nimbers(Game("grundy"), 20),
            (std::vector<std::uint32_t>{0, 0, 0, 1, 0, 2, 1, 0, 2, 1,
                                        0, 2, 1, 3, 2, 1, 3, 2, 4, 3}));
}
