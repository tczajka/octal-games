#include "game.h"
#include "gtest/gtest.h"

TEST(GameTest, test_names) {
  EXPECT_EQ(Game("0").name(), "0");
  EXPECT_EQ(Game("0.0").name(), "0");
  EXPECT_EQ(Game("4.0").name(), "4");
  EXPECT_EQ(Game("0.77").name(), ".77");
  EXPECT_EQ(Game("0.1234567").name(), ".1234567");
  EXPECT_EQ(Game(".1234567").name(), ".1234567");
  EXPECT_EQ(Game("grundy").name(), "grundy");
}

TEST(GameTest, test_moves) {
  const Game a{6,8,7};
  EXPECT_EQ(a.whole_moves(), 6u);
  EXPECT_EQ(a.take_moves(), 8u);
  EXPECT_EQ(a.split_moves(), 7u);
  EXPECT_TRUE(a.equal_split_allowed());
  EXPECT_EQ(a.name(), "4.552");

  const Game b("4.552");
  EXPECT_EQ(b.whole_moves(), 6u);
  EXPECT_EQ(b.take_moves(), 8u);
  EXPECT_EQ(b.split_moves(), 7u);
  EXPECT_TRUE(b.equal_split_allowed());

  const Game g = Game::grundy();
  EXPECT_EQ(g.whole_moves(), 0u);
  EXPECT_EQ(g.take_moves(), 0u);
  EXPECT_EQ(g.split_moves(), 1u);
  EXPECT_FALSE(g.equal_split_allowed());
}

TEST(GameTest, equality) {
  EXPECT_TRUE(Game("4.01") == Game("4.010"));
  EXPECT_TRUE(Game("4.01") != Game("4.02"));
}

TEST(GameTest, test_invalid) {
  EXPECT_THROW(Game("1.23"), std::domain_error);
  EXPECT_THROW(Game("12"), std::domain_error);
  EXPECT_THROW(Game("8.8"), std::domain_error);
  EXPECT_THROW(Game(1,2,2), std::domain_error);
  EXPECT_THROW(Game(2,1,2), std::domain_error);
}
