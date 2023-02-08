#include "dir/dir.h"

#include <dirent.h>

namespace common {

std::vector<std::pair<FileType, std::string>>
GetAllFilepath(
  const std::string& path,
  unsigned char file_type) {
  std::vector<std::pair<FileType, std::string>> res;
  DIR* dir = opendir(path.c_str());
  if (dir == nullptr) {
    return res;
  }
  struct dirent* entry = nullptr;
  while (nullptr != (entry = readdir(dir))) {
    if (entry->d_type == DT_REG && (file_type & kFileTypeReg)) {
      res.emplace_back(std::make_pair(kFileTypeReg, path + entry->d_name));
    } else if (entry->d_type == DT_DIR && (file_type & kFileTypeDir)) {
      res.emplace_back(std::make_pair(kFileTypeDir, path + entry->d_name));
    }
  }
  closedir(dir);
  return res;
}

}  // namespace common
