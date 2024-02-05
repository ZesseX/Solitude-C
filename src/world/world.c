#include "world.h"

static world_octree_node_t world_octrees[6];

// Return octree final node if it exists
// pos = index of chunk in the given chunk (coordinates / CHUNK_SIZE) 
world_octree_node_t* world_octree_get_final_node(int face, vec3_t pos) {
    world_octree_node_t* current_node = &world_octrees[face];
    
    int width = WORLD_OCTREE_SIZE_PER_AXIS;
    int half_width = width / 2;
    int start_x = 0, start_y = 0, start_z = 0;
    for (int i = 0; i < WORLD_OCTREE_DEPTH; i++) {
        int index = 0;
        index |= (pos.x > start_x + half_width) << 0;
        index |= (pos.y > start_y + half_width) << 1;
        index |= (pos.z > start_z + half_width) << 2;

        current_node = &world_octrees[index];
        if (current_node == NULL) {
            // TODO: Do something if no node exists (create it with world generation or load from file)
            return NULL;
        }

        if (i == WORLD_OCTREE_DEPTH) break;

        start_x = (index & (1 << 0)) ? start_x + half_width : start_x;
        start_y = (index & (1 << 1)) ? start_y + half_width : start_y;
        start_z = (index & (1 << 2)) ? start_z + half_width : start_z;

        width = half_width;
        half_width /= 2;
    }

    return current_node;
}

