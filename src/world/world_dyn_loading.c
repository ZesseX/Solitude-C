#include "world_dyn_loading.h"

// nodes must be final nodes with pointers to chunks
int world_dyn_loading_get_replacable(vec3_t pos, world_view_settings_t wvs, world_octree_node_t** nodes, int** dirty_ids, int len_nodes) {
    // loop trough all nodes, find ids which should no longer be visible
    // mark them dirty 1 else 0

    int index = 0;
    for (int i = 0; i < len_nodes; i++) {
        // check if nodes[i] is within pos +- range on xyz axis
        // TODO: Use range_y, make changes elsewhere too
        for (int j = 0; j < wvs.range_xz * wvs.range_xz * wvs.range_xz; j++) {
            vec3_t node_pos = nodes[j]->pos;
            
            (*dirty_ids)[j] = 0;

            if (node_pos.x < pos.x - wvs.range_xz || node_pos.x > pos.x + wvs.range_xz) continue;
            if (node_pos.z < pos.z - wvs.range_xz || node_pos.z > pos.z + wvs.range_xz) continue;
            if (node_pos.y < pos.y - wvs.range_xz || node_pos.y > pos.y + wvs.range_xz) continue;

            (*dirty_ids)[j] = 1;
        }
    }
}

int world_dyn_loading_load_new_nodes(world_octree_node_t** nodes, int** dirty_ids, int len_nodes) {
    // TODO: Only send in indices of the dirty ones not a list of 0s or 1s showing weather or not node is to be replaced
    for (int i = 0; i < dirty_ids; i++) {
        world_octree_node_t* parent_node = nodes[i]->parent;
        while(1) {
            int child_count = 0;
            for (int j = 0; j < 8; j++) {
                if (parent_node->world_octree_ptr.children[j] == NULL) continue;
                if (++child_count > 1) break;
            }

            // do not free this node if more than one child node depends on it (dirty node + other(s))
            if (child_count > 1) break;
            
            parent_node = parent_node->parent;    
        }

        // free the dirty node
        world_octree_node_t* final_parent = parent_node;
        parent_node = nodes[i]->parent;
        free_world_octree_node(nodes[i]->parent, 1);
        while(1) {
            world_octree_node_t* temp = parent_node;
            free_world_octree_node(parent_node, 0);
            if (final_parent == temp) break;
        }
    }
}
