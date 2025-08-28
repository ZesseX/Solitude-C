#include "world_octree.h"

void free_world_octree_node(world_octree_node_t* node, const int final) {
    if (final) {
        for (int i = 0; i < 8; i++) {
            free(node->world_octree_ptr.chunks[i]);
        }
    }
    else {
        for (int i = 0; i < 8; i++) {
            free(node->world_octree_ptr.children[i]);
        }
    }

    free(node);
}
