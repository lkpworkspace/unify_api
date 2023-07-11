/****************************************************************************
Copyright (c) 2023, likepeng
All rights reserved.

Author: likepeng <likepeng0418@163.com>
****************************************************************************/
#pragma once
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
  bool SetOwnerRecvTimeout(int timeout_ms);

  bool SetRecvTimeout(cmd_channel_handle_t fd, int timeout_ms);
  bool SetNonblock(cmd_channel_handle_t, bool);
  bool IsBlock(cmd_channel_handle_t);

  void CreateSockpair();
  void CloseSockpair();

  bool owner_block_{true};
  int owner_timeout_ms_{-1};
  cmd_channel_handle_t sockpair_[2] {-1, -1};

  DISALLOW_COPY_AND_ASSIGN(CmdChannel)
};

}  // namespace unify_api
