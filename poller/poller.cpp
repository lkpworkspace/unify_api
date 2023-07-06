/****************************************************************************
Copyright (c) 2023, likepeng
All rights reserved.

Author: likepeng <likepeng0418@163.com>
****************************************************************************/
#include "poller.h"

#include <cerrno>

namespace unify_api {

#if defined(UNIFY_API_OS_WINDOWS)
// TODO(likepeng)
#elif defined(UNIFY_API_OS_MAC)
// TODO(likepeng)
#else
static PollerEventType ToPollerEventType(uint32_t ev) {
  switch (ev) {
  case EPOLLIN:
    return PollerEventType::kIn;
  case EPOLLOUT:
    return PollerEventType::kOut;
  default:
    return PollerEventType::kErr;
  }
}

static uint32_t ToEpollType(const PollerEventType& ev) {
  switch (ev) {
  case PollerEventType::kIn:
    return EPOLLIN;
  case PollerEventType::kOut:
    return EPOLLOUT;
  default:
    return EPOLLERR;
  }
}
#endif

Poller::~Poller() {
#if defined(UNIFY_API_OS_WINDOWS)
// TODO(likepeng)
#elif defined(UNIFY_API_OS_MAC)
  if (poll_fd_ != -1) {
    close(poll_fd_);
    poll_fd_ = -1;
  }
  if (re_evs_ != nullptr) {
    free(re_evs_);
    re_evs_ = nullptr;
  }
  std::lock_guard<std::mutex> lock(mtx_);
  evs_.clear();
#else
  if (poll_fd_ != -1) {
    close(poll_fd_);
    poll_fd_ = -1;
  }
  if (evs_ != nullptr) {
    free(evs_);
    evs_ = nullptr;
  }
#endif
  init_.store(false);
}

bool Poller::Init(std::string* err_msg) {
  if (init_.load()) {
    return true;
  }
#if defined(UNIFY_API_OS_WINDOWS)
// TOOD(likepeng)
#elif defined(UNIFY_API_OS_MAC)
  poll_fd_ = kqueue();
  if (-1 == poll_fd_) {
    if (err_msg != nullptr) {
      *err_msg = "kqueue create() failed.";
    }
    return false;
  }
  evs_.reserve(max_ev_count_);
  auto void_re_evs = malloc(sizeof(struct kevent) * max_ev_count_);
  re_evs_ = reinterpret_cast<struct kevent*>(void_re_evs);
#else
  poll_fd_ = epoll_create(1024);
  if (-1 == poll_fd_) {
    if (err_msg != nullptr) {
      *err_msg = "poller create() failed.";
    }
    return false;
  }
  auto void_evs = malloc(sizeof(struct epoll_event) * max_ev_count_);
  evs_ = reinterpret_cast<struct epoll_event*>(void_evs);
#endif
  init_.store(true);
  return true;
}

bool Poller::Add(const PollerEvent& poller_ev, std::string* err_msg) {
  if (!init_.load()) {
    return false;
  }
#if defined(UNIFY_API_OS_WINDOWS)
// TODO(likepeng)
#elif defined(UNIFY_API_OS_MAC)
  (void)err_msg;
  struct kevent kev;
  switch (poller_ev.event) {
  case PollerEventType::kIn:
    EV_SET(&kev, poller_ev.fd, EVFILT_READ, EV_ADD, 0, 0, nullptr);
    break;
  case PollerEventType::kOut:
    EV_SET(&kev, poller_ev.fd, EVFILT_WRITE, EV_ADD, 0, 0, nullptr);
    break;
  case PollerEventType::kErr:
  default:
    return false;
  }
  std::lock_guard<std::mutex> lock(mtx_);
  add_ev_list_.push_back(kev);
#else
  struct epoll_event event;
  event.data.fd = poller_ev.fd;
  event.events = ToEpollType(poller_ev.event);
  int res = 0;
  // 如果该事件已经注册，就修改事件类型
  res = epoll_ctl(poll_fd_, EPOLL_CTL_MOD, poller_ev.fd, &event);
  if (-1 == res) {
    // 没有注册就添加至epoll
    res = epoll_ctl(poll_fd_, EPOLL_CTL_ADD, poller_ev.fd, &event);
    if (-1 == res) {
      if (err_msg!= nullptr) {
        *err_msg = std::string("epoll ") + strerror(errno);
      }
      return false;
    }
  } else {
    if (err_msg!= nullptr) {
      *err_msg = std::string(" has already reg ev ")
        + std::to_string(poller_ev.fd) + ": " + strerror(errno);
    }
    return false;
  }
#endif
  return true;
}

bool Poller::Del(int fd, std::string* err_msg) {
  if (!init_.load()) {
    return false;
  }
#if defined(UNIFY_API_OS_WINDOWS)
// TODO(likepeng)
#elif defined(UNIFY_API_OS_MAC)
  (void)err_msg;
  std::lock_guard<std::mutex> lock(mtx_);
  del_ev_list_.push_back(fd);
#else
  if (-1 == epoll_ctl(poll_fd_, EPOLL_CTL_DEL, fd, NULL)) {
    if (err_msg!= nullptr) {
      *err_msg = std::string("del event ")
        + std::to_string(fd) + ": " + strerror(errno);
    }
    return false;
  }
#endif
  return true;
}

int Poller::Wait(std::vector<PollerEvent>* evs, int timeout_ms, std::string* err_msg) {
  if (!init_.load()) {
    return -1;
  }
  evs->clear();
#if defined(UNIFY_API_OS_WINDOWS)
  // TODO(likepeng)
  (void)err_msg;
  int ev_count = 0;
#elif defined(UNIFY_API_OS_MAC)
  (void)err_msg;
  struct timespec ts;
  if (timeout_ms <  0) {
    ts.tv_sec = INT_MAX;
    ts.tv_nsec = 0;
  } else {
    ts.tv_sec = timeout_ms / 1000;
    ts.tv_nsec = (timeout_ms % 1000) * 1000000;
  }
  int ev_count = kevent(poll_fd_, &evs_[0], evs_.size(),
    re_evs_, max_ev_count_, &ts);
  if (0 > ev_count) {
    return -1;
  }
  for (int i = 0; i < ev_count; ++i) {
    PollerEvent poller_ev;
    poller_ev.fd = re_evs_[i].ident;
    if (re_evs_[i].filter == EVFILT_READ) {
      poller_ev.event = PollerEventType::kIn;
    } else if (re_evs_[i].filter == EVFILT_WRITE) {
      poller_ev.event = PollerEventType::kOut;
    } else {
      continue;
    }
    evs->push_back(poller_ev);
  }
  std::lock_guard<std::mutex> lock(mtx_);
  for (const auto& del_fd : del_ev_list_) {
    for (auto it = evs_.begin(); it != evs_.end();) {
      if (static_cast<int>(it->ident) == del_fd) {
        it = evs_.erase(it);
        continue;
      }
      ++it;
    }
  }
  for (const auto& it : add_ev_list_) {
    evs_.push_back(it);
  }
  add_ev_list_.clear();
  del_ev_list_.clear();
#else
  int ev_count = epoll_wait(poll_fd_, evs_, max_ev_count_, timeout_ms);
  if (0 > ev_count) {
    if (err_msg!= nullptr) {
      *err_msg = std::string("epoll wait error: ") + strerror(errno);
    }
    return -1;
  }
  for (int i = 0; i < ev_count; ++i) {
    PollerEvent poller_ev;
    poller_ev.fd = evs_[i].data.fd;
    poller_ev.event = ToPollerEventType(evs_[i].events);
    evs->push_back(poller_ev);
  }
#endif
  return ev_count;
}

}  // namespace unify_api
