/****************************************************************************
Copyright (c) 2023, likepeng
All rights reserved.

Author: likepeng <likepeng0418@163.com>
****************************************************************************/
#pragma once
#include <mutex>
#include <stdexcept>
#include <string>
#include "platform.h"
#if defined(UNIFY_API_OS_WINDOWS)
#include <Windows.h>
#else
#include <dlfcn.h>
#endif
#include "export.h"

namespace unify_api {

class UNIFY_API_EXPORT SharedLibrary {
 public:
  enum class Flags : int {
    // On platforms that use dlopen(), use RTLD_GLOBAL. This is the default
    // if no flags are given.
    kGlobal = 1,

    // On platforms that use dlopen(), use RTLD_LOCAL instead of RTLD_GLOBAL.
    //
    // Note that if this flag is specified, RTTI (including dynamic_cast and
    // throw) will not work for types defined in the shared library with GCC
    // and possibly other compilers as well. See
    // http://gcc.gnu.org/faq.html#dso for more information.
    kLocal = 2,
  };

  SharedLibrary() = default;
  virtual ~SharedLibrary();

 public:
  bool Load(const std::string& path, std::string* err_msg);
  bool Load(const std::string& path, Flags flags, std::string* err_msg);

  void Unload();

  bool IsLoaded();

  bool HasSymbol(const std::string& name);

  void* GetSymbol(const std::string& name, std::string* err_msg);

  inline const std::string& GetPath() const { return path_; }

 public:
  SharedLibrary(const SharedLibrary&) = delete;
  SharedLibrary& operator=(const SharedLibrary&) = delete;

 private:
#if defined(UNIFY_API_OS_WINDOWS)
  HMODULE handle_;
#else
  void* handle_{ nullptr };
#endif
  std::string path_;
  std::mutex mutex_;
};

}  // namespace unify_api
