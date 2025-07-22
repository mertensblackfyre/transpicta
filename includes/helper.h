#ifndef HELPER_H
#define HELPER_H

#include <algorithm>
#include <dirent.h>
#include <iostream>
#include <vector>
#include <spdlog/spdlog.h>

class Helper {
public:
  inline static void get_dirs(std::vector<std::string> &pages,
                              const std::string &path);
  inline static void get_files(std::vector<std::string> &pages,
                               const std::string &path,
                               const std::string &main_path);

  inline static std::string utils_get_extension(const std::string &f_name);
  inline static void sort_files(std::vector<std::string> &pages);
  inline static int extract_num(const std::string &fname);
};

inline void Helper::get_dirs(std::vector<std::string> &pages,
                             const std::string &path) {

  struct dirent *dir;
  DIR *dp = opendir(path.c_str());

  if (!dp) {
    spdlog::error("{} does not exist or cannot be opened", path.c_str());
    return;
  }

  while ((dir = readdir(dp)) != nullptr) {
    if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
      continue;

    if (dir->d_type == DT_DIR) {
      spdlog::info("Traversing {}", dir->d_name);
      get_files(pages, dir->d_name, path);

      continue;
    };
  }
  closedir(dp);
};

inline void Helper::get_files(std::vector<std::string> &pages,
                              const std::string &path,
                              const std::string &main_path) {
  struct dirent *dir;
  std::string ss = main_path + "/" + path;
  DIR *dp = opendir(ss.c_str());

  if (!dp) {
    spdlog::error("{} does not exist or cannot be opened", ss.c_str());
    return;
  }

  while ((dir = readdir(dp)) != nullptr) {
    if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
      continue;

    if (dir->d_type == DT_DIR) {
      continue;
    };
    std::string final_path = path + "/" + dir->d_name;
    if (!final_path.empty()) {
      pages.push_back(dir->d_name);
    }
  }
  closedir(dp);
};

inline int Helper::extract_num(const std::string &fname) {

  int num;
  size_t hyphen_pos = fname.find('-');
  if (hyphen_pos == std::string::npos)
    return 0;
  try {
    num = std::stoi(fname.substr(0, hyphen_pos));
  } catch (std::exception error_) {
    spdlog::error("{}", error_.what());
    return -1;
  }
  return num;
};

inline std::string Helper::utils_get_extension(const std::string &f_name) {
  size_t dot_pos = f_name.find('.');
  if (dot_pos == std::string::npos)
    return "";
  std::string ext = "";
  try {
    ext = f_name.substr(dot_pos, f_name.size() - 1);
  } catch (std::exception error_) {
    spdlog::error("{}", error_.what());
    return ext;
  }
  return ext;
};
#endif
