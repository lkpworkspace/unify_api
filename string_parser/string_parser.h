/****************************************************************************
Copyright (c) 2023, likepeng
All rights reserved.

Author: likepeng <likepeng0418@163.com>
****************************************************************************/
#pragma once
#include <vector>
#include <string>
#include <sstream>

namespace unify_api {

std::vector<std::string> SplitStr(const std::string& str, const char delim) {
  std::vector<std::string> key_nums;
  std::string item;
  std::stringstream ss(str);
  while (std::getline(ss, item, delim)) {
    key_nums.emplace_back(item);
  }
  return key_nums;
}

std::string JoinStr(
    std::initializer_list<std::string> str_list,
    const char delim = '\0') {
  std::string res;
  if (str_list.size() == 0) {
    return res;
  }
  for (const std::string& it : str_list) {
    res += it;
    if (delim != '\0') {
      res += delim;
    }
  }
  res.pop_back();
  return res;
}

}  // namespace unify_api
