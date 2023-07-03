/****************************************************************************
Copyright (c) 2023, likepeng
All rights reserved.

Author: likepeng <likepeng0418@163.com>
****************************************************************************/
#include "dir/dir.h"

namespace unify_api {

std::vector<std::pair<FileType, std::string>>
GetAllFilepath(const std::string& path) {
  std::vector<std::pair<FileType, std::string>> res;
  if (!stdfs::is_directory(path)) {
    return res;
  }
  for (const auto& entry : stdfs::directory_iterator(path)) {
    FileType type;
    if (entry.is_directory()) {
      type = FileType::kDir;
    } else if (entry.is_regular_file()) {
      type = FileType::kReg;
    } else {
      continue;
    }
    res.emplace_back(type, entry.path().string());
  }
  return res;
}

}  // namespace unify_api
