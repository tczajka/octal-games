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
