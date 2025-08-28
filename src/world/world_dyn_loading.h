#ifndef WORLD_DYN_LOADING_H
#define WORLD_DYN_LOADING_H

#include "world_octree.h"
#include "world_view_settings.h"

int world_dyn_loading_get_replacable(vec3_t pos, world_view_settings_t wvs, world_octree_node_t** nodes, int** dirty_ids, int len_nodes);

int world_dyn_loading_load_new_nodes(world_octree_node_t** nodes, int** dirty_ids, int len_nodes);

#endif
