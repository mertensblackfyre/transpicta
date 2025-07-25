#ifndef TRANSPICTA_H
#define TRANSPICTA_H

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <jpeglib.h>
#include <png.h>
#include <webp/decode.h>

class Transpicta {
public:
  inline static void transpicta_save_png(const char *filename, uint8_t *rgba,
                                         int width, int height);

  inline static void transpicta_save_jpeg(const char *filename, uint8_t *rgba,
                                          int width, int height);
};

void Transpicta::transpicta_save_jpeg(const char *filename, uint8_t *rgba,
                                      int width, int height) {
  FILE *outfile = fopen(filename, "wb");

  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;

  JSAMPROW row_pointer[1];
  int row_stride = width * 3;

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);

  jpeg_stdio_dest(&cinfo, outfile);

  cinfo.image_width = width;
  cinfo.image_height = height;
  cinfo.input_components = 3; // RGB
  cinfo.in_color_space = JCS_RGB;

  jpeg_set_defaults(&cinfo);
  jpeg_start_compress(&cinfo, TRUE);

  uint8_t *row = new uint8_t[row_stride];

  while (cinfo.next_scanline < cinfo.image_height) {
    for (int x = 0; x < width; x++) {
      row[x * 3 + 0] = rgba[cinfo.next_scanline * width * 4 + x * 4 + 0]; // R
      row[x * 3 + 1] = rgba[cinfo.next_scanline * width * 4 + x * 4 + 1]; // G
      row[x * 3 + 2] = rgba[cinfo.next_scanline * width * 4 + x * 4 + 2]; // B
    }
    row_pointer[0] = row;
    jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }

  jpeg_finish_compress(&cinfo);
  jpeg_destroy_compress(&cinfo);
  fclose(outfile);
  delete[] row;
};
void Transpicta::transpicta_save_png(const char *filename, uint8_t *rgba,
                                     int width, int height) {
  FILE *fp = fopen(filename, "wb");
  png_structp png =
      png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  png_infop info = png_create_info_struct(png);
  png_init_io(png, fp);

  png_set_IHDR(png, info, width, height, 8, PNG_COLOR_TYPE_RGBA,
               PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
               PNG_FILTER_TYPE_DEFAULT);
  png_write_info(png, info);

  png_bytep rows[height];
  for (int y = 0; y < height; y++)
    rows[y] = rgba + y * width * 4;

  png_write_image(png, rows);
  png_write_end(png, nullptr);
  fclose(fp);
  png_destroy_write_struct(&png, &info);
};

#endif
