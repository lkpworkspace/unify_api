/****************************************************************************
Copyright (c) 2023, likepeng
All rights reserved.

Author: likepeng <likepeng0418@163.com>
****************************************************************************/
#pragma once
#include <atomic>
#include "platform/cmd_channel.h"
#include "macros/singleton.h"

namespace unify_api {

class UNIFY_API_EXPORT CmdChannel final : public ICmdChannel {
 public:
  CmdChannel();
  virtual ~CmdChannel();

  cmd_channel_handle_t GetOwnerHandle() const override;
  cmd_channel_handle_t GetMainHandle() const override;

  int SendToOwner(const Cmd& cmd) override;
  int RecvFromOwner(Cmd* cmd, bool wait = false) override;

  int SendToMain(const Cmd& cmd) override;
  int RecvFromMain(Cmd* cmd, int timeout_ms = -1) override;

 private:
  cmd_channel_handle_t owner_handle_{nullptr};
  cmd_channel_handle_t main_handle_{nullptr};
  std::atomic<ICmdChannel::Cmd> owner_cmd_;
  std::atomic<ICmdChannel::Cmd> main_cmd_;

  DISALLOW_COPY_AND_ASSIGN(CmdChannel)
};

}  // namespace unify_api
