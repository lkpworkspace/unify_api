/****************************************************************************
Copyright (c) 2023, likepeng
All rights reserved.

Author: likepeng <likepeng0418@163.com>
****************************************************************************/
#pragma once
#include <atomic>
#include <mutex>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "unify_api_platform.h"
#include "macros/singleton.h"

#if defined(UNIFY_API_OS_WINDOWS)
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#elif defined(UNIFY_API_OS_MAC)
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <unistd.h>
#else
#include <sys/epoll.h>
#endif

namespace unify_api {

enum class PollerEventType : int {
  kErr = -1,
  kIn = 1,
  kOut = 2,
};

struct PollerEvent {
  int fd;
  PollerEventType event;
};

class Poller final {
 public:
  explicit Poller() = default;
  ~Poller();

  bool Init(std::string* err_msg = nullptr);
  bool Add(const PollerEvent&, std::string* err_msg = nullptr);
  bool Del(int fd, std::string* err_msg = nullptr);
  int Wait(std::vector<PollerEvent>* evs, int timeout_ms = 100);

 private:
  std::atomic_bool init_{false};
#if defined(UNIFY_API_OS_WINDOWS)
// TODO(likepeng)
#elif defined(UNIFY_API_OS_MAC)
  std::mutex mtx_;
  int poll_fd_{-1};
  size_t max_ev_count_{64};
  std::vector<struct kevent> add_ev_list_;
  std::vector<int> del_ev_list_;
  std::vector<struct kevent> evs_;
  struct kevent* re_evs_{nullptr};
#else
  int poll_fd_{-1};
  int max_ev_count_{64};
  struct epoll_event* evs_{nullptr};
#endif

  DISALLOW_COPY_AND_ASSIGN(Poller)
};

}  // namespace unfiy_api
