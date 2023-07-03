/****************************************************************************
Copyright (c) 2023, likepeng
All rights reserved.

Author: likepeng <likepeng0418@163.com>
****************************************************************************/
#pragma once
#include <cstdlib>
#include <string>

namespace unify_api {

// from stdlib:
// int setenv(const char *name, const char *value, int overwrite);
// int unsetenv(const char *name);

inline const std::string GetEnv(const std::string& key) {
  char* var = nullptr;
  var = std::getenv(key.c_str());
  if (var == nullptr) {
    return std::string("");
  }
  return std::string(var);
}

}  // namespace unify_api
