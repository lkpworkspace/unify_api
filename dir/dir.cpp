/****************************************************************************
Copyright (c) 2023, likepeng
All rights reserved.

Author: likepeng <likepeng0418@163.com>
****************************************************************************/
#include "dir/dir.h"
#ifdef UNIFY_API_OS_WINDOWS
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define MAX_PATH 512

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

stdfs::path GetSelfPath() {
  char path[MAX_PATH];
  memset(path, 0, sizeof(path));
#ifdef UNIFY_API_OS_WINDOWS
  HMODULE hMod = GetModuleHandel(NULL);
  DWORD len = GetModuleFileName(hMod, path, sizeof(path));
  if (len == 0) {
    return stdfs::path();
  }
#else
  int len = readlink("proc/self/exe", path, sizeof(path));
  if (len == -1) {
    return  stdfs::path();
  }
#endif
  if (static_cast<std::size_t>(len) >= sizeof(path)) {
    path[sizeof(path) - 1] = '\0';
  }
  stdfs::path p(path);
  if (p.has_parent_path()) {
    p = p.parent_path();
  }
  return p;
}

stdfs::path GetCurWorkPath() {
  char path[MAX_PATH];
  memset(path, 0, sizeof(path));
#ifdef UNIFY_API_OS_WINDOWS
  DWORD len = GetCurrentDirectory(MAX_PATH, path);
  if (len == 0) {
    return stdfs::path();
  }
#else
  if (getcwd(path, sizeof(path)) == nullptr) {
    return  stdfs::path();
  }
#endif
  return stdfs::path(path);
}

}  // namespace unify_api
