#include "world.h"

static world_octree_node_t world_octrees[6];

// TODO: Create vec3_t int version
static const vec3_t octree_offsets[8] = {
    {0, 0, 0},
    {1, 0, 0},
    {0, 1, 0},
    {1, 1, 0},
    {0, 0, 1},
    {1, 0, 1},
    {0, 1, 1},
    {1, 1, 1}
};

/*
* Octree indices
* 0 = (0, 0, 0) = 000
* 1 = (1, 0, 0) = 001
* 2 = (0, 1, 0) = 010
* 3 = (1, 1, 0) = 011
* 4 = (0, 0, 1) = 100
* 5 = (1, 0, 1) = 101
* 6 = (0, 1, 1) = 110
* 7 = (1, 1, 1) = 111
*/

void world_octrees_init_empty() {
    for (int i = 0; i < WORLD_OCTREE_FACE_COUNT; i++) {
        world_octrees[i].parent = NULL;
        world_octrees[i].world_octree_ptr.children = (world_octree_node_t**)malloc(sizeof(world_octree_node_t*) * 8);
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
        
        start_x = (index & (1 << 0)) ? start_x + half_width : start_x;
        start_y = (index & (1 << 1)) ? start_y + half_width : start_y;
        start_z = (index & (1 << 2)) ? start_z + half_width : start_z;
        printf("%d\n", half_width);
        temp_node = current_node->world_octree_ptr.children[index];
        if (temp_node == NULL) {
            world_octree_init_node(&temp_node);
            temp_node->parent = current_node;

            if (i == WORLD_OCTREE_DEPTH - 1){
                printf("XX\n");
                vec3_print((vec3_t){start_x, start_y, start_z});
                vec3_print(pos);
                printf("YY\n");
                world_octree_generate_final_node(temp_node, face, pos);
                current_node = temp_node;
                break;
            }
        }
        current_node = temp_node;

        width = half_width;
        half_width /= 2;
    }

    return current_node;
}

void world_octree_init_node(world_octree_node_t** node) {
    *node = (world_octree_node_t*)malloc(sizeof(world_octree_node_t));
    (*node)->world_octree_ptr.children = (world_octree_node_t**)malloc(sizeof(world_octree_node_t*) * 8);
    for (int i = 0; i < 8; i++) {
        (*node)->world_octree_ptr.children[i] = NULL;
    }
}

void world_octree_generate_final_node(world_octree_node_t* node, int face, vec3_t pos) {
    // TODO: Generate chunks based on position given
    chunk_t** chunks = (chunk_t**)malloc(sizeof(chunk_t*) * 8);
    node->world_octree_ptr.chunks = chunks;

    vec3_t real_pos = pos;
    for (int i = 0; i < 8; i++) {
        chunks[i] = (chunk_t*)malloc(sizeof(chunk_t));
        real_pos = vec3_add(pos, octree_offsets[i]);
        init_chunk(chunks[i], real_pos);
    }
}

void world_octree_get_chunks(chunk_t*** chunks, int range, vec3_t origin) {
    int single_axis = (range * 2 + 1); // range * 2 + 1 to get the size in one axis
    int chunk_count = single_axis * single_axis; // multiply by itself to get count of chunks in all axis

    (*chunks) = (chunk_t**)malloc(sizeof(chunk_t*) * chunk_count * 8); // multiply by 8 because octree has 8 chunks in the final node
    vec3_t pos;
    vec3_t start_pos = vec3_sub(origin, (vec3_t){range, range , range});
    int chunk_index = 0;
    for (int y = 0; y < single_axis; y++) {
        for (int x = 0; x < single_axis; x++) {
            for (int z = 0; z < single_axis; z++) {
                pos = vec3_add(start_pos, (vec3_t){x, y, z});

                world_octree_node_t* node = world_octree_get_final_node(0, pos);
                for (int i = 0; i < 8; i++) {
                    (*chunks)[chunk_index + i] = node->world_octree_ptr.chunks[i];
                }
                chunk_index += 8;
            }
        }
    }
}

int world_octree_get_face(vec3_t coords) {
    
}