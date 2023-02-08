#include <gtest/gtest.h>

#include "string_parser.h"

TEST(string_parser, test) {
  const char* str = "100, 101, 102";
  auto str_list = common::SpliteStr(str, ',');
  EXPECT_EQ(str_list.size(), 3);
  auto s = common::SplicingStr({"1", "2", "3"}, ',');
  EXPECT_EQ(s, "1,2,3");
}