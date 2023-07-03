/****************************************************************************
Copyright (c) 2023, likepeng
All rights reserved.

Author: likepeng <likepeng0418@163.com>
****************************************************************************/
#include "../env.h"

int main() {
  unsetenv("ENV_TEST");
  auto v = unify_api::GetEnv("ENV_TEST");
  if ("1" == v) {
    return -1;
  }
  setenv("ENV_TEST", "1", 1);
  v = unify_api::GetEnv("ENV_TEST");
  if ("1" != v) {
    return -1;
  }
  return 0;
}
