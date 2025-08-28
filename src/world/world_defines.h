#ifndef WORLD_DEFINES_H
#define WORLD_DEFINES_H

// Max height of perlin noise overlayed on top of the heightmap. (0 - this)
#define WORLD_MAX_PERLIN_HEIGHT 64

#define WORLD_OCTREE_FACE_COUNT 6
#define WORLD_OCTREE_DEPTH 17
#define WORLD_OCTREE_SIZE_PER_AXIS 262144

#define DEMO_WORLD_RANGE 1
#define DEMO_WORLD_RANGE_PER_AXIS (DEMO_WORLD_RANGE * 2 + 1)

#endif
