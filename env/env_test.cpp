#include <gtest/gtest.h>

#include "env.h"

TEST(env, test) {
  unsetenv("ENV_TEST");
  auto v = common::GetEnv("ENV_TEST");
  EXPECT_NE("1", v);
  setenv("ENV_TEST", "1", 1);
  v = common::GetEnv("ENV_TEST");
  EXPECT_EQ("1", v);
}
