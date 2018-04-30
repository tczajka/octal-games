#include "container_util.h"

#include "gtest/gtest.h"

TEST(ContainerUtilTest, test_map_find) {
  std::map<std::string, int> m{
    {"abc", 5},
    {"def", 6}};

  EXPECT_EQ(map_get(m, std::string("abc")), 5);
  EXPECT_EQ(map_get(m, std::string("def")), 6);
  EXPECT_THROW(map_get(m, std::string("foo")), std::runtime_error);
}
