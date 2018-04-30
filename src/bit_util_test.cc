#include "bit_util.h"
#include "gtest/gtest.h"

TEST(BitUtilTest, test_lowest_bit_32) {
  EXPECT_EQ(lowest_bit(std::uint32_t(1)), 0u);
  EXPECT_EQ(lowest_bit(std::uint32_t(0xff)), 0u);
  EXPECT_EQ(lowest_bit(std::uint32_t(0xf0)), 4u);
  EXPECT_EQ(lowest_bit(std::uint32_t(0x80000000)), 31u);
}

TEST(BitUtilTest, test_lowest_bit_64) {
  EXPECT_EQ(lowest_bit(std::uint64_t(1)), 0u);
  EXPECT_EQ(lowest_bit(std::uint64_t(0xff)), 0u);
  EXPECT_EQ(lowest_bit(std::uint64_t(0xf0)), 4u);
  EXPECT_EQ(lowest_bit(std::uint64_t(0x80000000)), 31u);
  EXPECT_EQ(lowest_bit(std::uint64_t(0xf00000000)), 32u);
  EXPECT_EQ(lowest_bit(std::uint64_t(0xf000000000000000)), 60u);
  EXPECT_EQ(lowest_bit(std::uint64_t(0x8000000000000000)), 63u);
}

TEST(BitUtilTest, test_get_bit) {
  EXPECT_EQ(get_bit(11u, 0), true);
  EXPECT_EQ(get_bit(11u, 1), true);
  EXPECT_EQ(get_bit(11u, 2), false);
  EXPECT_EQ(get_bit(11u, 3), true);
  EXPECT_EQ(get_bit(11u, 5), false);
  EXPECT_EQ(get_bit(1ULL<<63, 63), true);
}

TEST(BitUtilTest, test_set_bit) {
  std::uint32_t a = 0x10u;
  set_bit(a, 8);
  EXPECT_EQ(a, 0x110u);
  set_bit(a, 31);
  EXPECT_EQ(a, 0x80000110u);
  std::uint64_t b = 1u;
  set_bit(b, 32);
  EXPECT_EQ(b, 0x100000001u);
}

TEST(BitUtilTest, test_reset_bit) {
  std::uint32_t a = 0x10u;
  reset_bit(a, 8);
  EXPECT_EQ(a, 0x10u);
  reset_bit(a, 4);
  EXPECT_EQ(a, 0u);
  std::uint64_t b = 0xffffffffffffffffu;
  reset_bit(b, 63);
  EXPECT_EQ(b, 0x7fffffffffffffffu);
}
