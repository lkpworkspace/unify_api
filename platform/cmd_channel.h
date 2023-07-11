/****************************************************************************
Copyright (c) 2023, likepeng
All rights reserved.

Author: likepeng <likepeng0418@163.com>
****************************************************************************/
#pragma once
#include "unify_api_export.h"
#include "platform_define.h"

namespace unify_api {

class UNIFY_API_EXPORT ICmdChannel {
 public:
  enum class Cmd : char {
    kQuit = 'q',          ///< 退出
    kIdle = 'i',          ///< 空闲
    kWaitForMsg = 'w',    ///< 等待消息
    kRun = 'r',           ///< 运行
    kRunWithMsg = 'm',    ///< 运行(有消息)
  };

  virtual cmd_channel_handle_t GetOwnerHandle() const = 0;
  virtual cmd_channel_handle_t GetMainHandle() const = 0;

  virtual int SendToOwner(const Cmd& cmd) = 0;
  virtual int RecvFromOwner(Cmd* cmd, bool wait = false) = 0;

  virtual int SendToMain(const Cmd& cmd) = 0;
  virtual int RecvFromMain(Cmd* cmd, int timeout_ms = -1) = 0;
};

}  // namespace unify_api

#if defined(UNIFY_API_OS_WINDOWS)
#include "win/cmd_channel-win.h"
#else
#include "linux/cmd_channel-linux.h"
#endif
