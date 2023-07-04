/****************************************************************************
Copyright (c) 2023, likepeng
All rights reserved.

Author: likepeng <likepeng0418@163.com>
****************************************************************************/
#include "../string_parser.h"

int main() {
  const char* str = "100, 101, 102";
  auto str_list = unify_api::SplitStr(str, ',');
  if (str_list.size() != 3) {
    return -1;
  }
  auto s = unify_api::JoinStr({"1", "2", "3"}, ',');
  if (s != "1,2,3") {
    return -1;
  }
  return 0;
}
