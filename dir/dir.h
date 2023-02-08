#pragma once
#include <string>
#include <vector>

namespace common {

enum FileType {
  kFileTypeReg = 0x1,
  kFileTypeDir = 0x2,
};

/* eg: GetAllFilepath("/home/") */
std::vector<std::pair<FileType, std::string>>
GetAllFilepath(
  const std::string& path,
  unsigned char file_type = kFileTypeReg);

}  // namespace common
