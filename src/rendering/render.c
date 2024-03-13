#include "render.h"

GLFWwindow* window = NULL;

transform_t cam_trans;
camera_t camera;

//chunk_t** base_chunks;
//int render_block_count = 16;

float last_frame_time = 0;

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

    last_frame_time = glfwGetTime();

    render_init_render_chunks(render_context);
    
    return 1;
}

int renderer_update(render_context_t* render_context) {
    render_update_start();
    render_chunk(NULL);
    render_update_end();

    return glfwWindowShouldClose(window);
}

render_chunk_t* rcs;
typedef struct {
    GLuint shader_id;
    GLuint texture_id;

    GLuint mvp_matrix_loc;
    GLuint texture_loc;
} render_chunk_defaults_t;

render_chunk_defaults_t rc_defaults;

void render_init_render_chunks(render_context_t* render_context) {
    int scs;
    rc_defaults.shader_id = shader_load("shader/vert.vert", "shader/frag.frag", &scs);
    if(!scs){
        // TODO: Do something when loading / compiling the shader fails.
    }

    //base_chunks = (chunk_t**)malloc(sizeof(chunk_t*) * render_block_count);
    //for (int x = 0; x < render_block_count; x++) {
    //    base_chunks[x] = (chunk_t*)malloc(sizeof(chunk_t) * render_block_count);
    //    for (int z = 0; z < render_block_count; z++) {
    //        init_chunk(&base_chunks[x][z], (vec2_t){x, z});
    //    }
    //}

    //world_octree_node_t* wofn = world_octree_get_final_node(0, (vec3_t){0, 0, 0});
    chunk_t** chunks;
    world_octree_get_chunks(&chunks, 1, (vec3_t){5, 5, 5});

    glUseProgram(rc_defaults.shader_id);

    int rccount = 3 * 3 * 3 * 8;
    rcs = (render_chunk_t*)malloc(sizeof(render_chunk_t) * rccount);
    for (int i = 0; i < rccount; i++) {
        update_render_chunk(chunks[i], &rcs[i]);
    }

    texture_t* texture = load_texture("textures/32x32.tga");
    glGenerateMipmap(texture->id);

    rc_defaults.texture_id = texture->id;
    rc_defaults.mvp_matrix_loc = glGetUniformLocation(rc_defaults.shader_id, "mvp");;
    rc_defaults.texture_loc = glGetUniformLocation(rc_defaults.shader_id, "texture_sampler");

    camera.transform.position = (vec3_t){0.0f, 32.0f, 0.0f};
}

double interval = 0.1;
double interval_cur = 0.0;
int fps = 0;
void render_update_start() {
    vec2_t move_inputs = input_get_movement(window);
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

void render_chunk(chunk_t* chunk) {
    glUseProgram(rc_defaults.shader_id);
    mat4_t camera_mat = mat4_mul(camera.view, camera.proj);

    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            for(int z = 0; z < 3; z++) {
                for (int i = 0; i < 8; i++) {
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
    
                    vec3_t offset = octree_offsets[i];
                    int real_index = ((y * CHUNK_SIZE_SQUARED + x * CHUNK_SIZE + z) * 8) + i;
                    transform_t chunk_transform = {
                        .position = {(offset.x * CHUNK_SIZE) + x * 2 * CHUNK_SIZE, 
                                    (offset.y * CHUNK_SIZE) + y * 2 * CHUNK_SIZE, 
                                    (offset.z * CHUNK_SIZE) + z * 2 * CHUNK_SIZE},
                        .rotation = {0.0f, 0.0f, 0.0f},
                        .scale = {1.0f, 1.0f, 1.0f}
                    };
    
                    glBindVertexArray(rcs[real_index].vao);
                    glBindTexture(GL_TEXTURE_2D, rc_defaults.texture_id);
    
                    mat4_t mvp = mat4_mul(mat4_model(&chunk_transform), camera_mat);
                    glUniformMatrix4fv(rc_defaults.mvp_matrix_loc, 1, GL_FALSE, (float*)mvp.m);
                    glDrawElements(GL_TRIANGLES, rcs[real_index].indices, GL_UNSIGNED_INT, 0);
                }
            }
        }
    }

    glUseProgram(0);
}
