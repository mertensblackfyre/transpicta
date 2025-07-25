
#include "includes/decoder.h"
#include "includes/helper.h"
#include "includes/transpicta.h"
#include <spdlog/spdlog.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {

  if (argc < 4) {
    spdlog::error("[Usage]: transpicta [option] [files]");
    return -1;
  };

  std::string option = argv[1];
  std::string convert_to = argv[2];

  if (option == "-file") {
    std::string original_fname = argv[3];
    std::string new_fname = argv[4];
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
          Transpicta::transpicta_save_png(new_fname.c_str(),rgba,width,height);
        spdlog::info("Converted {} to png", original_fname);
        return 0;
      }
    };
  };

  if (option == "-dir") {
  };

  // Helper::get_dirs(d_name);
};
