/****************************************************************************
Copyright (c) 2023, likepeng
All rights reserved.

Author: likepeng <likepeng0418@163.com>
****************************************************************************/
#pragma once
#include <cstring>

#include <string>
#include <atomic>
#include <mutex>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "unify_api_export.h"
#include "unify_api_platform.h"
#include "macros/singleton.h"

#if defined(UNIFY_API_OS_WINDOWS)
#include <Windows.h>
typedef HANDLE handle_fd_t;
#elif defined(UNIFY_API_OS_MAC)
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <unistd.h>
typedef int handle_fd_t;
#else
#include <sys/epoll.h>
#include <unistd.h>
typedef int handle_fd_t;
#endif

namespace unify_api {

enum class PollerEventType : int {
  kErr = -1,
  kIn = 1,
  kOut = 2,
};

struct PollerEvent {
  handle_fd_t fd;
  PollerEventType event;
};

class UNIFY_API_EXPORT Poller final {
 public:
  explicit Poller() = default;
  ~Poller();

  bool Init(std::string* err_msg = nullptr);
  bool Add(const PollerEvent&, std::string* err_msg = nullptr);
  bool Del(handle_fd_t fd, std::string* err_msg = nullptr);
  int Wait(std::vector<PollerEvent>* evs,
    int timeout_ms = 100,
    std::string* err_msg = nullptr);

 private:
  std::atomic_bool init_{false};
#if defined(UNIFY_API_OS_WINDOWS)
  std::mutex mtx_;
  size_t max_ev_count_{64};
  std::unordered_set<handle_fd_t> add_evs_;
  std::unordered_set<handle_fd_t> del_evs_;
  std::vector<handle_fd_t> evs_;
#elif defined(UNIFY_API_OS_MAC)
  std::mutex mtx_;
  handle_fd_t poll_fd_{-1};
  size_t max_ev_count_{64};
  std::vector<struct kevent> add_ev_list_;
  std::vector<handle_fd_t> del_ev_list_;
  std::vector<struct kevent> evs_;
  struct kevent* re_evs_{nullptr};
#else
  handle_fd_t poll_fd_{-1};
  size_t max_ev_count_{64};
  struct epoll_event* evs_{nullptr};
#endif

  DISALLOW_COPY_AND_ASSIGN(Poller)
};

}  // namespace unify_api
