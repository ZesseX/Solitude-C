#ifndef WORLD_OCTREE_H
#define WORLD_OCTREE_H

#include "chunk.h"

typedef struct world_octree_node_t world_octree_node_t;
struct world_octree_node_t{
    world_octree_node_t* parent;
    vec3_t pos;

    // Use 'children' for all but the final node which should use 'chunks'
    union {
        world_octree_node_t** children;
        chunk_t** chunks;
    } world_octree_ptr;
};

// if final node then union uses 'chunks' => free them instead of 'childen'
void free_world_octree_node(world_octree_node_t* node, const int final);

#endif
