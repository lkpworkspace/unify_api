/****************************************************************************
Copyright (c) 2023, likepeng
All rights reserved.

Author: likepeng <likepeng0418@163.com>
****************************************************************************/
#include <iostream>
#include "../cmd_channel.h"

int main() {
  unify_api::CmdChannel cmd_channel;
  unify_api::ICmdChannel::Cmd cmd;

  cmd_channel.SendToMain(unify_api::ICmdChannel::Cmd::kIdle);
  cmd_channel.RecvFromOwner(&cmd, true);
  std::cout << "get owner cmd " << static_cast<char>(cmd) << std::endl;
  if (cmd != unify_api::ICmdChannel::Cmd::kIdle) {
    return -1;
  }

  int ret = cmd_channel.RecvFromMain(&cmd, 10);
  if (ret != 0) {
    return -1;
  }
  if (ret == 0) {
    std::cout << "get main cmd timeout" << std::endl;
  }

  cmd_channel.SendToOwner(unify_api::ICmdChannel::Cmd::kRunWithMsg);
  ret = cmd_channel.RecvFromMain(&cmd);
  if (ret <= 0) {
    return -1;
  }
  if (cmd != unify_api::ICmdChannel::Cmd::kRunWithMsg) {
    return -1;
  }
  std::cout << "get main cmd " << static_cast<char>(cmd) << std::endl;
  return 0;
}
