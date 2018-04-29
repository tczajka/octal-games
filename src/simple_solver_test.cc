#include "simple_solver.h"
#include "gtest/gtest.h"

TEST(SimpleSolverTest, test_mex) {
  EXPECT_EQ(mex({}), 0u);
  EXPECT_EQ(mex({2,1,2}), 0u);
  EXPECT_EQ(mex({0,3,0,1}), 2u);
  EXPECT_EQ(mex({0,3,1,0xffffffffu}), 2u);
}
