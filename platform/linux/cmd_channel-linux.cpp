/****************************************************************************
Copyright (c) 2023, likepeng
All rights reserved.

Author: likepeng <likepeng0418@163.com>
****************************************************************************/
#include "cmd_channel-linux.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

namespace unify_api {

CmdChannel::CmdChannel() {
  CreateSockpair();
}

CmdChannel::~CmdChannel() {
  CloseSockpair();
}

cmd_channel_handle_t CmdChannel::GetOwnerHandle() const {
  return sockpair_[0];
}

cmd_channel_handle_t CmdChannel::GetMainHandle() const {
  return sockpair_[1];
}

void CmdChannel::CreateSockpair() {
  int res = -1;
  res = socketpair(AF_UNIX, SOCK_DGRAM, 0, sockpair_);
  if (res) {
    return;
  }
  if (!SetNonblock(sockpair_[0], false)) {
    return;
  }
  if (!SetNonblock(sockpair_[1], false)) {
    return;
  }
}

void CmdChannel::CloseSockpair() {
  if (sockpair_[0] != -1) {
    close(sockpair_[0]);
    sockpair_[0] = -1;
  }
  if (sockpair_[1] != -1) {
    close(sockpair_[1]);
    sockpair_[1] = -1;
  }
}

int CmdChannel::SendToOwner(const Cmd& cmd) {
  char cmd_char = static_cast<char>(cmd);
  int ret = write(sockpair_[1], &cmd_char, 1);
  return ret;
}

int CmdChannel::RecvFromOwner(Cmd* cmd, bool wait) {
  (void)wait;
  char cmd_char;
  int ret = read(sockpair_[1], &cmd_char, 1);
  if (ret < 0) {
    return ret;
  }
  *cmd = static_cast<Cmd>(cmd_char);
  return ret;
}

int CmdChannel::RecvFromMain(Cmd* cmd, int timeout_ms) {
  if (!SetOwnerRecvTimeout(timeout_ms)) {
    return -1;
  }
  char cmd_char;
  int ret = read(sockpair_[0], &cmd_char, 1);
  if (ret < 0) {
    return ret;
  }
  *cmd = static_cast<Cmd>(cmd_char);
  return ret;
}

int CmdChannel::SendToMain(const Cmd& cmd) {
  char cmd_char = static_cast<char>(cmd);
  int ret = write(sockpair_[0], &cmd_char, 1);
  return ret;
}

bool CmdChannel::SetOwnerRecvTimeout(int timeout_ms) {
  bool flag = true;
  if (timeout_ms < 0) {
    // block and no timeout
    if (!owner_block_) {
      flag = SetNonblock(sockpair_[0], false);
      if (flag) {
        owner_block_ = true;
      }
    }
    if (owner_timeout_ms_ > 0) {
      flag = SetRecvTimeout(sockpair_[0], 0);
      if (flag) {
        owner_timeout_ms_ = -1;
      }
    }
  } else if (timeout_ms == 0) {
    // nonblock and no timeout
    if (owner_block_) {
      bool flag = SetNonblock(sockpair_[0], true);
      if (flag) {
        owner_block_ = false;
      }
    }
    if (owner_timeout_ms_ > 0) {
      flag = SetRecvTimeout(sockpair_[0], 0);
      if (flag) {
        owner_timeout_ms_ = -1;
      }
    }
  } else {
    // block and timeout
    if (!owner_block_) {
      flag = SetNonblock(sockpair_[0], false);
      if (flag) {
        owner_block_ = true;
      }
    }
    if (owner_timeout_ms_ != timeout_ms) {
      flag = SetRecvTimeout(sockpair_[0], timeout_ms);
      if (flag) {
        owner_timeout_ms_ = timeout_ms;
      }
    }
  }
  return flag;
}

bool CmdChannel::SetNonblock(cmd_channel_handle_t fd, bool b) {
  int flags = fcntl(fd, F_GETFL, 0);
  if (b) {
    flags |= O_NONBLOCK;
  } else {
    flags &= ~O_NONBLOCK;
  }
  return (fcntl(fd, F_SETFL, flags) != -1);
}

bool CmdChannel::IsBlock(cmd_channel_handle_t fd) {
  int flags = fcntl(fd, F_GETFL, 0);
  return !(flags & O_NONBLOCK);
}

bool CmdChannel::SetRecvTimeout(cmd_channel_handle_t fd, int timeout_ms) {
  struct timeval timeout = {timeout_ms / 1000, (timeout_ms % 1000) * 1000};
  return 0 == setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO,
    reinterpret_cast<const char*>(&timeout),
    sizeof(timeout));
}

}  // namespace unify_api
