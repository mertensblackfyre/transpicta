#include <iostream>

#include <spdlog/spdlog.h>

int main(int argc, char *argv[]) {

  if (argc > 3) {
    spdlog::error("[Usage]: transpicta [option] [directory]");
    return -1;
  };
  std::string d_name = argv[2];
  std::string option = argv[1];

  if (argv[1] == option) {
  };
};
