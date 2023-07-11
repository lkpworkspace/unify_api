/****************************************************************************
Copyright (c) 2023, likepeng
All rights reserved.

Author: likepeng <likepeng0418@163.com>
****************************************************************************/
#pragma once
#include <cstdlib>
#include <string>

#include "unify_api_platform.h"

namespace unify_api {

// from stdlib:
// int setenv(const char *name, const char *value, int overwrite);
// int unsetenv(const char *name);

inline const std::string GetEnv(const std::string& key) {
  char* var = nullptr;
#ifdef UNIFY_API_OS_WINDOWS
  size_t len = 0;
  _dupenv_s(&var, &len, key.c_str());
#else
  var = getenv(key.c_str());
#endif
  if (var == nullptr) {
    return std::string("");
  }
  return std::string(var);
}

inline void SetEnv(
    const std::string& key,
    const std::string& value,
    bool overwrite = true) {
#ifdef UNIFY_API_OS_WINDOWS
  _putenv_s(key.c_str(), value.c_str());
#else
  setenv(key.c_str(), value.c_str(), overwrite);
#endif
}

}  // namespace unify_api
