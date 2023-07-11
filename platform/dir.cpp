/****************************************************************************
Copyright (c) 2023, likepeng
All rights reserved.

Author: likepeng <likepeng0418@163.com>
****************************************************************************/
#include "platform/dir.h"
#include <cstring>
#include <iostream>
#include "unify_api_platform.h"
#ifdef UNIFY_API_OS_WINDOWS
#include <Windows.h>
#elif defined(UNIFY_API_OS_MAC)
#include <unistd.h>
#include <mach-o/dyld.h>
#else
#include <unistd.h>
#endif

#ifndef MAX_PATH
#define MAX_PATH 512
#endif

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
  HMODULE hMod = GetModuleHandle(NULL);
  DWORD len = GetModuleFileName(hMod, path, sizeof(path));
  if (len == 0) {
    return stdfs::path();
  }
#elif defined(UNIFY_API_OS_MAC)
  char exec_path[MAX_PATH];
  uint32_t len = sizeof(exec_path);
  if (_NSGetExecutablePath(exec_path, &len)) {
    return stdfs::path();
  }
  if (realpath(exec_path, path) == nullptr) {
    return stdfs::path();
  }
#else
  int len = readlink("proc/self/exe", path, sizeof(path));
  if (len == -1) {
    return stdfs::path();
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
