#ifndef WORLD_H
#define WORLD_H

#include "world_defines.h"
#include "world_octree.h"
#include "chunk.h"

void world_octree_init_empty();
world_octree_node_t* world_octree_get_final_node(int face, vec3_t pos);
void world_octree_init_node(world_octree_node_t** node);
void world_octree_generate_final_node(world_octree_node_t* node, int face, vec3_t pos);

void world_octree_get_chunks(chunk_t*** chunks, int range, vec3_t origin);

#endif