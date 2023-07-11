/****************************************************************************
Copyright (c) 2023, likepeng
All rights reserved.

Author: likepeng <likepeng0418@163.com>
****************************************************************************/
#include "platform/shared_library.h"

namespace unify_api {

SharedLibrary::~SharedLibrary() {
  Unload();
}

bool SharedLibrary::Load(const std::string& path, std::string* err_msg) {
  return Load(path, Flags::kGlobal, err_msg);
}

bool SharedLibrary::Load(
    const std::string& path,
    Flags flags,
    std::string* err_msg) {
  std::lock_guard<std::mutex> lock(mutex_);
  if (handle_) {
    if (err_msg) {
      *err_msg = std::string("SharedLibrary already loaded: ") + path;
    }
    return false;
  }
#if defined(UNIFY_API_OS_WINDOWS)
  handle_ = LoadLibrary(path.c_str());
  if (handle_ == nullptr) {
    if (err_msg) {
      *err_msg = "failed to load " + path;
    }
    return false;
  }
#else
  int real_flag = RTLD_LAZY;
  if (static_cast<int>(flags) & static_cast<int>(Flags::kLocal)) {
    real_flag |= RTLD_LOCAL;
  } else {
    real_flag |= RTLD_GLOBAL;
  }
  handle_ = dlopen(path.c_str(), real_flag);
  if (!handle_) {
    if (err_msg) {
      const char* err = dlerror();
      *err_msg = (err ? std::string(err) : path);
    }
    return false;
  }
#endif
  path_ = path;
  return true;
}

void SharedLibrary::Unload() {
  std::lock_guard<std::mutex> lock(mutex_);
#if defined(UNIFY_API_OS_WINDOWS)
  if (handle_) {
    FreeLibrary(handle_);
    handle_ = nullptr;
  }
#else
  if (handle_) {
    dlclose(handle_);
    handle_ = nullptr;
  }
#endif
}

bool SharedLibrary::IsLoaded() {
  std::lock_guard<std::mutex> lock(mutex_);
  return handle_ != nullptr;
}

bool SharedLibrary::HasSymbol(const std::string& name) {
  return GetSymbol(name, nullptr) != nullptr;
}

void* SharedLibrary::GetSymbol(const std::string& name, std::string* err_msg) {
  std::lock_guard<std::mutex> lock(mutex_);
  if (!handle_) return nullptr;
#if defined(UNIFY_API_OS_WINDOWS)
  void* result = GetProcAddress(handle_, name.c_str());
  if (!result) {
    if (err_msg) {
      *err_msg = name;
    }
    return nullptr;
  }
#else
  void* result = dlsym(handle_, name.c_str());
  if (!result) {
    const char* err = dlerror();
    if (err_msg) {
      *err_msg = (err ? std::string(err) : name);
    }
    return nullptr;
  }
#endif
  return result;
}

}  // namespace unify_api
