/****************************************************************************
Copyright (c) 2023, likepeng
All rights reserved.

Author: likepeng <likepeng0418@163.com>
****************************************************************************/
#pragma once
#include <string>
#include <vector>
#include <utility>
#if __has_include(<filesystem>)
#include <filesystem>
namespace stdfs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
#include <experimental/filesystem>
namespace stdfs = std::experimental::filesystem;
#else
#error "no filesytem found"
#endif

namespace unify_api {

enum class FileType : int {
  kReg = 0x1,
  kDir = 0x2,
};

/* eg: GetAllFilepath("/home/") */
std::vector<std::pair<FileType, std::string>>
GetAllFilepath(const std::string& path);

}  // namespace unify_api
