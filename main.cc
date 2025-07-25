
#include <iostream>
#include <cxxopts.hpp>
#include <spdlog/spdlog.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {

  try {
    cxxopts::Options options("Transpicta", "Convert images between formats");

    options.add_options()("f,file", "Input file",
                          cxxopts::value<std::string>())(
        "d,dir", "Directory mode (not implemented here)",
        cxxopts::value<std::string>()->default_value(""))(
        "j,jpeg", "Convert to JPEG", cxxopts::value<std::string>())(
        "p,png", "Convert to PNG", cxxopts::value<std::string>())(
        "w,webp", "Convert to WEBP",
        cxxopts::value<std::string>())("h,help", "Print help");

    auto result = options.parse(argc, argv);

    if (result.count("help") || argc == 1) {
      std::cout << options.help() << std::endl;
      return 0;
    }

    if (!result.count("file")) {
      spdlog::error("Input file is required. Use --file <filename>");
      return 1;
    }

    std::string input_file = result["file"].as<std::string>();
    if (result.count("jpeg")) {
      std::string out = result["jpeg"].as<std::string>();
      spdlog::info("Converting {} to JPEG as {}", input_file, out);
    } else if (result.count("png")) {
      std::string out = result["png"].as<std::string>();
      spdlog::info("Converting {} to PNG as {}", input_file, out);
    } else if (result.count("webp")) {
      std::string out = result["webp"].as<std::string>();
      spdlog::info("Converting {} to WEBP as {}", input_file, out);
    } else {
      spdlog::error(
          "No output format specified. Use --jpeg, --png, or --webp.");
      return 1;
    }

  } catch (const cxxopts::exceptions::exception &e) {
    spdlog::error("Error parsing options: {}", e.what());
    return 1;
  }

  /*
*
*
if (option == "-file") {
std::string ext = Helper::get_extension(original_fname);
if (ext == "webp") {
int width, height;
uint8_t *rgba =
 Decoder::decoder_webp(original_fname.c_str(), &width, &height);
if (convert_to == "-jpeg") {
Transpicta::transpicta_save_jpeg(new_fname.c_str(), rgba, width,
                                height);
spdlog::info("Converted {} to jpeg", original_fname);
return 0;
};
if (convert_to == "-png") {
Transpicta::transpicta_save_png(new_fname.c_str(), rgba, width, height);
spdlog::info("Converted {} to png", original_fname);
return 0;
}
};
};
*/

  // Helper::get_dirs(d_name);
};
