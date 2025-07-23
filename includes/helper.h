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
                               const std::string &output_dir);

  inline static std::string utils_get_extension(const std::string &f_name);
  inline static std::string helper_create_dir(const std::string &dir_name,
                                              const std::string &suffix);
};

inline void Helper::get_dirs(const std::string &path) {

  std::string output_dir = helper_create_dir(path, "-jpeg");
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
      std::string tmp2 = output_dir + "/" + dir->d_name;
      std::string tmp = path + "/" + dir->d_name;
      std::string rr = helper_create_dir(tmp2, "");
      get_files("JPEG", tmp, rr);
      continue;
    };
  }
  closedir(dp);
};

inline void Helper::get_files(const std::string &option,
                              const std::string &path,
                              const std::string &output_dir) {

  struct dirent *dir;
  std::string ss = path;

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
    std::string new_path =
        output_dir + "/" + utils_get_extension(dir->d_name) + ".jpg";

    if (!final_path.empty()) {
      int wid, len;
      uint8_t *rgba = Decoder::decoder_webp(final_path.c_str(), &wid, &len);
      Transpicta::transpicta_save_jpeg(new_path.c_str(), rgba, wid, len);
      spdlog::info("Successfully converted {} to ", dir->d_name);
    }
  }
  closedir(dp);
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

inline std::string Helper::helper_create_dir(const std::string &dir_name,
                                             const std::string &suffix) {

  std::string s = dir_name + suffix;
  try {
    if (std::filesystem::create_directories(s)) {
      spdlog::info("Directory {} created successfully", s);
    } else {
      spdlog::warn("Directory {} already exists or could not be created.", s);
      return "";
    }
  } catch (const std::filesystem::filesystem_error &e) {
    spdlog::error("Error creating directory: {} ", e.what());
    return "";
  };

  return s;
};
#endif
