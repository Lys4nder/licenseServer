#include <gtest/gtest.h>

TEST(Tests, Test1) {
  EXPECT_EQ(1, 1);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}