/****************************************************************************
Copyright (c) 2023, likepeng
All rights reserved.

Author: likepeng <likepeng0418@163.com>
****************************************************************************/
#include <iostream>
#include "../poller.h"

int main(int argc, char** argv) {
  unify_api::Poller poller;
  if (!poller.Init()) {
    return -1;
  }
#if defined(UNIFY_API_OS_WINDOWS)
  // TODO(likepeng)
#else
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
      std::cout << "Failed to create socket. Error code: "
        << errno << std::endl;
      return -1;
  }
  unify_api::PollerEvent ev;
  ev.fd = sockfd;
  ev.event = unify_api::PollerEventType::kIn;
  if (!poller.Add(ev)) {
    std::cout << "add event failed" << std::endl;
    return -1;
  }
#endif

  std::vector<unify_api::PollerEvent> items;
  int cnt = poller.Wait(&items);
  std::cout << "wait cnt: " << cnt << std::endl;

  std::cout << "polling end." << std::endl;
  return 0;
}
