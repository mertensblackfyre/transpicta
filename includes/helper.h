#ifndef HELPER_H
#define HELPER_H

#include "decoder.h"
#include "transpicta.h"
#include <algorithm>
#include <dirent.h>
#include <filesystem>
#include <iostream>
#include <spdlog/spdlog.h>
#include <vector>

class Helper {
public:
  inline static void get_dirs(const std::string &path);
  inline static void get_files(const std::string &option,
                               const std::string &path,
                               const std::string &m_path);

  inline static std::string utils_get_extension(const std::string &f_name);
  inline static void sort_files(std::vector<std::string> &pages);
  inline static int extract_num(const std::string &fname);
};

inline void Helper::get_dirs(const std::string &path) {

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
      get_files("JPEG", dir->d_name, path);
      continue;
    };
  }
  closedir(dp);
};

inline void Helper::get_files(const std::string &option,
                              const std::string &path,
                              const std::string &m_path) {
  std::string s = path + option;
  try {
    if (std::filesystem::create_directory(s)) {
      spdlog::info("Directory {} created successfully", s);
    } else {
      spdlog::warn("Directory {} already exists or could not be created.", s);
    }
  } catch (const std::filesystem::filesystem_error &e) {
    spdlog::warn("Error creating directory: ", e.what());
  };

  struct dirent *dir;
  std::string ss = m_path + "/" + path;

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
    std::string new_path = s + "/" + utils_get_extension(dir->d_name) + ".jpg";
    if (!final_path.empty()) {

      int wid, len;
     // uint8_t *rgba = Decoder::decoder_webp(final_path.c_str(), &wid, &len);
   //   Transpicta::transpicta_save_jpeg(new_path.c_str(), rgba, wid, len);

      std::cout << "GREAT" << std::endl;
      std::cout << new_path << std::endl;
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
    ext = f_name.substr(0, dot_pos);
  } catch (std::exception error_) {
    spdlog::error("{}", error_.what());
    return ext;
  }
  return ext;
};
#endif
