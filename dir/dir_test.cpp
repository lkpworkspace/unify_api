#include <gtest/gtest.h>

#include "dir.h"

TEST(dir, test) {
  auto file_list = common::GetAllFilepath("/home/caros/", 0x2);
  for (const auto& it : file_list) {
    std::cout << it.second << std::endl;
  }
}
