#include "tga.h"

tga_image_t* load_tga(const char* name){
    FILE* file = fopen(name, "rb");
    if (!file){
        fprintf(stderr, "Unable to open .tga file: %s\n", name);
        return NULL;
    }

    // header is 18 bytes long
    uint8_t header[18];
    fread(header, 1, sizeof(header), file);

    tga_image_t img;
    img.type = header[2];
    img.width = *(int16_t*)&header[12];
    img.height = *(int16_t*)&header[14];
    img.bit_depth = header[16];

    printf("%u\n", img.bit_depth);

    size_t img_size = img.width * img.height * (img.bit_depth / 8);
    img.data = (uint8_t*)malloc(img_size);
    fread(img.data, 1, img_size, file);

    fclose(file);

    tga_image_t* final = (tga_image_t*)malloc(sizeof(tga_image_t));
    *final = img;

    return final;
}

void free_tga_data(tga_image_t* img){
    free(img->data);
}
