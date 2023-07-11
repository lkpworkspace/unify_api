/****************************************************************************
Copyright (c) 2023, likepeng
All rights reserved.

Author: likepeng <likepeng0418@163.com>
****************************************************************************/
#include "cmd_channel-win.h"

namespace unify_api {

CmdChannel::CmdChannel() {
  owner_handle_ = CreateEvent(nullptr, false, false, nullptr);
  main_handle_ = CreateEvent(nullptr, false, false, nullptr);
}

CmdChannel::~CmdChannel() {
  if (owner_handle_ != nullptr) {
    CloseHandle(owner_handle_);
    owner_handle_ = nullptr;
  }
  if (main_handle_ != nullptr) {
    CloseHandle(main_handle_);
    main_handle_ = nullptr;
  }
}

cmd_channel_handle_t CmdChannel::GetOwnerHandle() const {
  return owner_handle_;
}

cmd_channel_handle_t CmdChannel::GetMainHandle() const {
  return main_handle_;
}

int CmdChannel::SendToOwner(const Cmd& cmd) {
  owner_cmd_.store(cmd);
  if (!SetEvent(owner_handle_)) {
    return -1;
  }
  return 1;
}

int CmdChannel::RecvFromOwner(Cmd* cmd, bool wait) {
  if (!wait) {
    *cmd = main_cmd_.load();
    return 1;
  }
  DWORD ret = WaitForSingleObject(main_handle_, INFINITE);
  if (ret == WAIT_OBJECT_0) {
    *cmd = main_cmd_.load();
  } else {
    return -1;
  }
  return 1;
}

int CmdChannel::SendToMain(const Cmd& cmd) {
  main_cmd_.store(cmd);
  if (!SetEvent(main_handle_)) {
    return -1;
  }
  return 1;
}

int CmdChannel::RecvFromMain(Cmd* cmd, int timeout_ms) {
  DWORD ret = WaitForSingleObject(owner_handle_, timeout_ms);
  if (ret == WAIT_OBJECT_0) {
    *cmd = owner_cmd_.load();
  } else if (ret == WAIT_TIMEOUT) {
    return 0;
  } else if (ret == WAIT_FAILED) {
    return -1;
  }
  return 1;
}

}  // namespace unify_api
