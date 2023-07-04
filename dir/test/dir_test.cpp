/****************************************************************************
Copyright (c) 2023, likepeng
All rights reserved.

Author: likepeng <likepeng0418@163.com>
****************************************************************************/
#include <iostream>
#include "../dir.h"

int main() {
  auto file_list = unify_api::GetAllFilepath("/");
  for (const auto& it : file_list) {
    std::cout << static_cast<int>(it.first) << ":" << it.second << std::endl;
  }

  auto self_path = unify_api::GetSelfPath();
  std::cout << self_path.string() << std::endl;

  auto work_path = unify_api::GetCurWorkPath();
  std::cout << work_path.string() << std::endl;

  return 0;
}
