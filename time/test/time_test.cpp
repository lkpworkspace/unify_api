/****************************************************************************
Copyright (c) 2023, likepeng
All rights reserved.

Author: likepeng <likepeng0418@163.com>
****************************************************************************/
#include <iostream>
#include "../duration.h"
#include "../rate.h"
#include "../time.h"

int main() {
  auto now = unify_api::Time::Now();
  std::cout << "now: " << now << std::endl;
  return 0;
}
