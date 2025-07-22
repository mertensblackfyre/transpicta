#ifndef DECODER_H
#define DECODER_H

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <webp/decode.h>

class Decoder {
public:
  inline uint8_t *decoder_webp(const char *file, int *width, int *height);
};

uint8_t *Decoder::decoder_webp(const char *file, int *width, int *height) {
  FILE *f = fopen(file, "rb");
  fseek(f, 0, SEEK_END);
  size_t size = ftell(f);
  rewind(f);

  uint8_t *data = (uint8_t *)malloc(size);
  fread(data, size, 1, f);
  fclose(f);

  uint8_t *rgba = WebPDecodeRGBA(data, size, width, height);
  free(data);
  return rgba;
};
#endif
