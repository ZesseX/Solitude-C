#ifndef WORLD_OCTREE_H
#define WORLD_OCTREE_H

#include "chunk.h"

typedef struct world_octree_node_t world_octree_node_t;
struct world_octree_node_t{
    world_octree_node_t* parent;

    // Use 'children' for all but the final node which should use 'chunks'
    union {
        world_octree_node_t** children;
        chunk_t** chunks;
    } world_octree_ptr;
};



#endif
