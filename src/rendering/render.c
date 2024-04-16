#include "render.h"

GLFWwindow* window = NULL;

transform_t cam_trans;
camera_t camera;

int renderer_init(render_context_t* render_context){
    if (!glfwInit()) {
        printf("Failed to initialize glfw\n");
        return 0;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Solitude", NULL, NULL);
    if (!window) {

        printf("Failed to create glfw window\n");
        glfwTerminate();
        return 0;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize glad\n");
        glfwTerminate();
        return 0;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEBUG_OUTPUT);

    opengl_debug_init(OGL_DEBUG_FLAG_HIGH | OGL_DEBUG_FLAG_MEDIUM | OGL_DEBUG_FLAG_LOW);
    glDebugMessageCallback(opengl_debug_msg_callback, NULL);

    render_init_render_chunks(render_context);
    
    return 1;
}

int renderer_update(render_context_t* render_context) {
    render_update_start();
    render_chunk(render_context);
    render_update_end();

    return glfwWindowShouldClose(window);
}

render_chunk_t* rcs;

void render_init_render_chunks(render_context_t* rc) {
    int scs;
    rc->render_chunk_context.shader_id = shader_load("shader/vert.vert", "shader/frag.frag", &scs);
    if(!scs){
        // TODO: Do something when loading / compiling the shader fails.
    }

    chunk_t** chunks;
    world_octree_get_chunks(&chunks, DEMO_WORLD_RANGE, (vec3_t){DEMO_WORLD_RANGE, DEMO_WORLD_RANGE, DEMO_WORLD_RANGE});

    glUseProgram(rc->render_chunk_context.shader_id);

    int rccount = DEMO_WORLD_RANGE_PER_AXIS * DEMO_WORLD_RANGE_PER_AXIS * DEMO_WORLD_RANGE_PER_AXIS * 8;
    rcs = (render_chunk_t*)malloc(sizeof(render_chunk_t) * rccount);

    for (int y = 0; y < DEMO_WORLD_RANGE_PER_AXIS; y++) {
        for (int x = 0; x < DEMO_WORLD_RANGE_PER_AXIS; x++) {
            for (int z = 0; z < DEMO_WORLD_RANGE_PER_AXIS; z++) {
                int index = (y * DEMO_WORLD_RANGE_PER_AXIS * DEMO_WORLD_RANGE_PER_AXIS) + (x * DEMO_WORLD_RANGE_PER_AXIS) + z;
                index *= 8;
                for (int i = 0; i < 8; i++) {
                    
                    update_render_chunk(chunks[index], &rcs[index]);
                    index++;
                }
            }
        }
    }

    texture_t* texture_0 = load_texture("textures/32x32_0.tga");
    glGenerateMipmap(texture_0->id);

    texture_t* texture_1 = load_texture("textures/32x32_1.tga");
    glGenerateMipmap(texture_1->id);

    rc->render_chunk_context.texture_ids = (GLuint*)malloc(TEXTURE_ARRAY_LENGTH * sizeof(GLuint));

    rc->render_chunk_context.texture_ids[0] = texture_0->id;
    rc->render_chunk_context.texture_ids[1] = texture_1->id;
    
    rc->render_chunk_context.mvp_matrix_loc = glGetUniformLocation(rc->render_chunk_context.shader_id, "mvp");;
    rc->render_chunk_context.texture_loc = glGetUniformLocation(rc->render_chunk_context.shader_id, "texture_sampler");

    camera.transform.position = (vec3_t){0.0f, 32.0f, 0.0f};
}

double interval = 0.1;
double interval_cur = 0.0;
int fps = 0;
void render_update_start() {
    vec3_t move_inputs = input_get_movement(window);
    vec2_t rotation_inputs = input_get_rotation(window);

    if (interval_cur <= 0.0) {
        fps = (int)(1.0 / get_deltatime());
        interval_cur = interval;
    }
    else {
        interval_cur -= get_deltatime();
    }

    char* fps_text = (char*)malloc(sizeof(char) * 50);
    snprintf(fps_text, 50, "%d", fps);
    glfwSetWindowTitle(window, fps_text);
    camera_update_transform_freecam(&camera, get_deltatime(), move_inputs, rotation_inputs);

    glClearColor(0.1, 0.3f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void render_update_end() {
    glfwSwapBuffers(window);
    glfwPollEvents();
} 

void render_chunk(render_context_t* rc) {
    rc->render_chunk_context.texture_swap_interval -= interval_cur;
    if (rc->render_chunk_context.texture_swap_interval <= 0) {
        rc->render_chunk_context.texture_swap_interval = TEXTURE_SWAP_INTERVAL;
        rc->render_chunk_context.texture_index++;
        if (rc->render_chunk_context.texture_index > TEXTURE_ARRAY_LENGTH - 1) {
            rc->render_chunk_context.texture_index = 0;
        }
    }

    glUseProgram(rc->render_chunk_context.shader_id);
    mat4_t camera_mat = mat4_mul(camera.view, camera.proj);

    const vec3_t octree_offsets[8] = {
        {0, 0, 0},
        {1, 0, 0},
        {0, 1, 0},
        {1, 1, 0},
        {0, 0, 1},
        {1, 0, 1},
        {0, 1, 1},
        {1, 1, 1}
    };

    for (int y = 0; y < DEMO_WORLD_RANGE_PER_AXIS; y++) {
        for (int x = 0; x < DEMO_WORLD_RANGE_PER_AXIS; x++) {
            for (int z = 0; z < DEMO_WORLD_RANGE_PER_AXIS; z++) {
                
                for (int i = 0; i < 8; i++) {
                    int index = (y * DEMO_WORLD_RANGE_PER_AXIS * DEMO_WORLD_RANGE_PER_AXIS) + (x * DEMO_WORLD_RANGE_PER_AXIS) + z;
                    vec3_t pos = rcs[index * 8 + i].chunk->position;

                    vec3_t offset = octree_offsets[i];
                    offset = vec3_mulf(offset, CHUNK_SIZE);

                    vec3_t real_pos = pos;
                    transform_t chunk_transform = {
                        .position = real_pos,
                        .rotation = {0.0f, 0.0f, 0.0f},
                        .scale = {1.0f, 1.0f, 1.0f}
                    };

                    glBindVertexArray(rcs[index * 8 + i].vao);
                    glBindTexture(GL_TEXTURE_2D, rc->render_chunk_context.texture_ids[rc->render_chunk_context.texture_index]);

                    mat4_t mvp = mat4_mul(mat4_model(&chunk_transform), camera_mat);
                    glUniformMatrix4fv(rc->render_chunk_context.mvp_matrix_loc, 1, GL_FALSE, (float*)mvp.m);
                    glDrawElements(GL_TRIANGLES, rcs[index * 8 + i].indices, GL_UNSIGNED_INT, 0);
                }
            }
        }
    }

    glUseProgram(0);
}
