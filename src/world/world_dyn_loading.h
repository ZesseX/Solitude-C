#ifndef WORLD_DYN_LOADING_H
#define WORLD_DYN_LOADING_H

#include "world_octree.h"

int world_dyn_load_get_replacable(vec3_t pos, world_octree_node_t** nodes, int** dirty_ids);

#endif
