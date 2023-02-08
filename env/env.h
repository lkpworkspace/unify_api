#pragma once
#include <stdlib.h>
#include <string>

namespace common {

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

}  // namespace common