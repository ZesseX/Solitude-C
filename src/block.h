#ifndef BLOCK_H
#define BLOCK_H

#include <stdint.h>

struct block_t {
    uint32_t id;
    void* data;
};

#endif