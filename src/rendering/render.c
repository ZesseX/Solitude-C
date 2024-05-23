#include "render.h"

camera_t camera;

int renderer_init(render_context_t* rc){
    if (!glfwInit()) {
        printf("Failed to initialize glfw\n");
        return 0;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    rc->window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Solitude", NULL, NULL);
    if (!rc->window) {

        printf("Failed to create glfw window\n");
        glfwTerminate();
        return 0;
    }

    glfwMakeContextCurrent(rc->window);
    glfwSwapInterval(0);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize glad\n");
        glfwTerminate();
        return 0;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEBUG_OUTPUT);

    opengl_debug_init(OGL_DEBUG_FLAG_HIGH | OGL_DEBUG_FLAG_MEDIUM | OGL_DEBUG_FLAG_LOW);
    glDebugMessageCallback(opengl_debug_msg_callback, NULL);

    render_init_render_chunks(rc);
    
    return 1;
}

int renderer_update(render_context_t* rc) {
    render_update_start(rc);
    render_chunk(rc);
    render_update_end(rc);

    return glfwWindowShouldClose(rc->window);
}

void render_init_render_chunks(render_context_t* rc) {
    int scs;
    rc->render_chunk_context.shader_id = shader_load("shader/vert.vert", "shader/frag.frag", &scs);
    if(!scs){
        // TODO: Do something when loading / compiling the shader fails.
    }

    chunk_t** chunks;
    world_octree_get_chunks(&chunks, DEMO_WORLD_RANGE, (vec3_t){DEMO_WORLD_RANGE, DEMO_WORLD_RANGE, DEMO_WORLD_RANGE});

    glUseProgram(rc->render_chunk_context.shader_id);

    int num_rcs = DEMO_WORLD_RANGE_PER_AXIS * DEMO_WORLD_RANGE_PER_AXIS * DEMO_WORLD_RANGE_PER_AXIS * 8;
    rc->render_chunks = (render_chunk_t*)malloc(sizeof(render_chunk_t) * num_rcs);

    const int num_threads = 8;
    pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t) * num_threads);
    rc_create_thread_data_t* thread_data = (rc_create_thread_data_t*)malloc(sizeof(rc_create_thread_data_t) * num_threads);

    int chunks_per_thread = num_rcs / num_threads;
    int remaining_chunks = num_rcs % num_threads;
    
    for (int i = 0; i < num_threads; i++) {
        // TODO: Save memory by only giving chunk / rc pointers to the indices required
        thread_data[i].chunks = chunks;
        thread_data[i].rcs = rc->render_chunks;
        thread_data[i].start_index = i * chunks_per_thread;
        thread_data[i].end_index = (i + 1) * chunks_per_thread;

        thread_data[i].verts = (GLfloat**)malloc(sizeof(GLfloat*) * num_rcs);
        thread_data[i].indices = (GLuint**)malloc(sizeof(GLuint*) * num_rcs);

        thread_data[i].num_verts = (int*)malloc(sizeof(int) * num_rcs);
        thread_data[i].num_indices = (int*)malloc(sizeof(int) * num_rcs);

        if (i == num_threads - 1) {
            // TODO: Divide evenly between first x chunks, do not give all to last one
            thread_data[i].end_index += remaining_chunks;
        }
        pthread_create(&threads[i], NULL, rc_create_func, (void*)&thread_data[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    int thread_index = 0;
    for (int i = 0; i < num_rcs; i++) {
        if (i > thread_data[thread_index].end_index) thread_index++;
        rc->render_chunks[i] = thread_data[thread_index].rcs[i];
        update_render_chunk_opengl(chunks[i], &rc->render_chunks[i], 
            &(thread_data[thread_index].verts[i]), &(thread_data[thread_index].indices[i]), 
            thread_data[thread_index].num_verts[i], thread_data[thread_index].num_indices[i]);
    }

    free(threads);
    free(thread_data);

    rc->render_chunk_count = num_rcs;

    rc->texture_animation_main_atlas.anim_frame_time = 1.0;
    rc->texture_animation_main_atlas.cur_anim_frame_index = 0;
    rc->texture_animation_main_atlas.cur_anim_frame_time = 0.0;

    (void)texture_animations_load_textures(&rc->texture_animation_main_atlas, 2, "textures/32x32");

    rc->render_chunk_context.mvp_matrix_loc = glGetUniformLocation(rc->render_chunk_context.shader_id, "mvp");;
    rc->render_chunk_context.texture_loc = glGetUniformLocation(rc->render_chunk_context.shader_id, "texture_sampler");

    camera.transform.position = (vec3_t){0.0f, 32.0f, 0.0f};
}

double interval = 0.1;
double interval_cur = 0.0;
int fps = 0;
void render_update_start(render_context_t* rc) {
    vec3_t move_inputs = input_get_movement(rc->window);
    vec2_t rotation_inputs = input_get_rotation(rc->window);

    if (interval_cur <= 0.0) {
        fps = (int)(1.0 / get_deltatime());
        interval_cur = interval;
    }
    else {
        interval_cur -= get_deltatime();
    }

    char* fps_text = (char*)malloc(sizeof(char) * 50);
    snprintf(fps_text, 50, "%d", fps);
    glfwSetWindowTitle(rc->window, fps_text);
    camera_update_transform_freecam(&camera, get_deltatime(), move_inputs, rotation_inputs);

    glClearColor(0.1, 0.3f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void render_update_end(render_context_t* rc) {
    glfwSwapBuffers(rc->window);
    glfwPollEvents();
} 

void render_chunk(render_context_t* rc) {
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
                    vec3_t pos = rc->render_chunks[index * 8 + i].chunk->position;

                    vec3_t offset = octree_offsets[i];
                    offset = vec3_mulf(offset, CHUNK_SIZE);

                    vec3_t real_pos = pos;
                    transform_t chunk_transform = {
                        .position = real_pos,
                        .rotation = {0.0f, 0.0f, 0.0f},
                        .scale = {1.0f, 1.0f, 1.0f}
                    };

                    glBindVertexArray(rc->render_chunks[index * 8 + i].vao);
                    glBindTexture(GL_TEXTURE_2D, texture_animation_get_current_id(&rc->texture_animation_main_atlas));

                    mat4_t mvp = mat4_mul(mat4_model(&chunk_transform), camera_mat);
                    glUniformMatrix4fv(rc->render_chunk_context.mvp_matrix_loc, 1, GL_FALSE, (float*)mvp.m);
                    glDrawElements(GL_TRIANGLES, rc->render_chunks[index * 8 + i].indices, GL_UNSIGNED_INT, 0);
                }
            }
        }
    }

    glUseProgram(0);
}

void* rc_create_func(void* args) {
    rc_create_thread_data_t* data = (rc_create_thread_data_t*)args;
    for (int i = data->start_index; i < data->end_index; i++) {
        update_render_chunk_init(data->chunks[i], &data->rcs[i], &(data->verts[i]), &(data->indices[i]), &(data->num_verts[i]), &(data->num_indices[i]));
    }
    //free(data);
    return NULL;
}
