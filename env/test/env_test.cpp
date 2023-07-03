/****************************************************************************
Copyright (c) 2023, likepeng
All rights reserved.

Author: likepeng <likepeng0418@163.com>
****************************************************************************/
#include "../env.h"

int main() {
  unify_api::SetEnv("ENV_TEST", "2");
  auto v = unify_api::GetEnv("ENV_TEST");
  if ("1" == v) {
    return -1;
  }
  unify_api::SetEnv("ENV_TEST", "1");
  v = unify_api::GetEnv("ENV_TEST");
  if ("1" != v) {
    return -1;
  }
  return 0;
}
