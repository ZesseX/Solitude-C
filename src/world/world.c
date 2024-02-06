#include "world.h"

static world_octree_node_t world_octrees[6];

void world_octrees_init_empty() {
    for (int i = 0; i < WORLD_OCREE_FACE_COUNT; i++) {
        world_octrees[i].parent = NULL;
        world_octrees[i].world_octree_ptr.children = (world_octree_node_t**)malloc(sizeof(world_octree_node_t**));
        for (int j = 0; j < 8; j++) {
            world_octrees[i].world_octree_ptr.children[j] = NULL;
        }
    }
}

// Return octree final node if it exists
// pos = index of chunk in the given chunk (coordinates / CHUNK_SIZE) 
world_octree_node_t* world_octree_get_final_node(int face, vec3_t pos) {
    world_octree_node_t* current_node = &world_octrees[face];
    world_octree_node_t* temp_node;

    int width = WORLD_OCTREE_SIZE_PER_AXIS;
    int half_width = width / 2;
    int start_x = 0, start_y = 0, start_z = 0;
    for (int i = 0; i < WORLD_OCTREE_DEPTH; i++) {
        int index = 0;
        index |= (pos.x > start_x + half_width) << 0;
        index |= (pos.y > start_y + half_width) << 1;
        index |= (pos.z > start_z + half_width) << 2;

        temp_node = current_node->world_octree_ptr.children[index];
        if (temp_node == NULL) {
            world_octree_init_node(temp_node);
            temp_node->parent = current_node;

            if (i == WORLD_OCTREE_DEPTH - 1){
                world_octree_generate_final_node(temp_node, face, pos);
                break;
            }
        }
        current_node = temp_node;

        start_x = (index & (1 << 0)) ? start_x + half_width : start_x;
        start_y = (index & (1 << 1)) ? start_y + half_width : start_y;
        start_z = (index & (1 << 2)) ? start_z + half_width : start_z;

        width = half_width;
        half_width /= 2;
    }

    return current_node;
}

void world_octree_init_node(world_octree_node_t* node) {
    node = (world_octree_node_t*)malloc(sizeof(world_octree_node_t*));
    node->world_octree_ptr.children = (world_octree_node_t**)malloc(sizeof(world_octree_node_t**));
    for (int i = 0; i < 8; i++) {
        node->world_octree_ptr.children[i] = NULL;
    }
}

void world_octree_generate_final_node(world_octree_node_t* node, int face, vec3_t pos) {
    // TODO: Generate chunks based on position given
}
