#ifndef CHUNK_H
#define CHUNK_H

#include <stdint.h>

#define CHUNK_SIZE_XZ 16
#define CHUNK_SIZE_Y 256

struct chunk_t {
    uint32_t* blocks;
};

#endif