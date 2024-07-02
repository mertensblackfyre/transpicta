#ifndef FILE_H
#define FILE_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class File {

private:
  std::string header =
      "| Header 1 | Header 2 | Header 3 |\n|----------|----------|----------|";

public:
  void file_read();
  void file_write_header();
  void file_check_header();
};

void File::file_write_header() {
  std::ofstream fp;
  fp.open("/home/mertens/Documents/notes/Others/test.md", std::ios_base::app);
  fp << header;
  fp.close();
};

void File::file_read() {
  std::ifstream nfp;
  std::string str;
  nfp.open("/home/mertens/Documents/notes/Others/test.md");
  std::ostringstream ss;
  ss << nfp.rdbuf();
  std::string fileContent = ss.str();

  size_t hpos = fileContent.find(header);
  if (hpos == std::string::npos) {

    file_write_header();
  };
};
#endif
