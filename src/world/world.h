#ifndef WORLD_H
#define WORLD_H

#include "world_defines.h"
#include "chunk.h"

/*
* World consists of 6 planes, each of those six planes is an octree (depth 9 with 38m chunk size per axis)
* 38 * ??? => 9 961 472m (9961km)
* world circumference 39 845km
*/

// TODO: Remove some of the Y axis chunks?, no real need for having chunks at y = ~10 000 km
// Mariana Trench depth + Mount Everest = ~20km

#define WORLD_OCTREE_FACE_COUNT 6
#define WORLD_OCTREE_DEPTH 17
#define WORLD_OCTREE_SIZE_PER_AXIS 262144

typedef struct world_octree_node_t world_octree_node_t;
struct world_octree_node_t{
    world_octree_node_t* parent;

    // Use 'children' for all but the final node which should use 'chunks'
    union {
        world_octree_node_t** children;
        chunk_t** chunks;
    } world_octree_ptr;
};

void world_octrees_init_empty();
world_octree_node_t* world_octree_get_final_node(int face, vec3_t pos);
void world_octree_init_node(world_octree_node_t** node);
void world_octree_generate_final_node(world_octree_node_t* node, int face, vec3_t pos);

void world_octree_get_chunks(chunk_t*** chunks, int range, vec3_t origin);

int world_octree_get_face(vec3_t coords);

#endif