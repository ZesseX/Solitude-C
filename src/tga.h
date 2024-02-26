#ifndef TGA_H
#define TGA_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint8_t type;
    int16_t width;
    int16_t height;
    uint8_t bit_depth;
    uint8_t* data;
} tga_image_t;

tga_image_t* load_tga(const char* name);

void free_tga_data(tga_image_t* img);

#endif
