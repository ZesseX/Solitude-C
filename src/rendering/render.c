#include "render.h"

GLFWwindow* window = NULL;

transform_t cam_trans;
camera_t camera;

render_block_t* default_render_blocks;
chunk_t* base_chunks;
int render_block_count = 1024;

float last_frame_time = 0;

int renderer_init(){
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
    glfwSwapInterval(0);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize glad\n");
        glfwTerminate();
        return 0;
    }

    glEnable(GL_DEPTH_TEST);

    last_frame_time = glfwGetTime();

    render_init_temp_chunk();
    
    return 1;
}

int renderer_update() {
    render_update_start();
    render_chunk(NULL);
    render_update_end();

    return glfwWindowShouldClose(window);
}

void render_init_temp_chunk() {
    int scs;
    //default_render_block.shader_id = shader_load("shader/vert.vert", "shader/frag.frag", &scs);
    //default_render_block2.shader_id = default_render_block.shader_id;
    //printf("Program id: %u\n", default_render_block.shader_id);

    GLuint shader_id = shader_load("shader/vert.vert", "shader/frag.frag", &scs);

    default_render_blocks = (render_block_t*)malloc(sizeof(render_block_t) * render_block_count);
    base_chunks = (chunk_t*)malloc(sizeof(chunk_t) * render_block_count);
    for (int i = 0; i < render_block_count; i++) {
        init_chunk(&base_chunks[i]);
    }

    int vertex_count;
    GLfloat* vertices = get_chunk_vertices(&vertex_count);
    
    texture_t* texture = load_texture("textures/32x32.tga");
    glGenerateMipmap(texture->id);

    GLuint vbo;
    glUseProgram(shader_id);
    for (int i = 0; i < render_block_count; i++){
        default_render_blocks[i].shader_id = shader_id;

        GLuint* indices = get_chunk_block_indices(&base_chunks[i]);

        GLuint vao, ebo;
        glGenVertexArrays(1, &vao);
        if(i == 0) {
            glGenBuffers(1, &vbo);
        }
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        if(i == 0) {
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertex_count, vertices, GL_STATIC_DRAW);
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * base_chunks[i].index_count, indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        default_render_blocks[i].vao = vao;
        default_render_blocks[i].vbo = vbo;
        default_render_blocks[i].ebo = ebo;

        default_render_blocks[i].test_matrix_loc = glGetUniformLocation(shader_id, "test");
        default_render_blocks[i].texture_loc = glGetUniformLocation(shader_id, "texture_sampler");
        default_render_blocks[i].texture_id = texture->id;
    }
}

void render_update_start() {
    float cur_frame_time = glfwGetTime();
    float deltatime = cur_frame_time - last_frame_time;
    last_frame_time = cur_frame_time;

    float aspect_ratio = (float)WINDOW_WIDTH / (float)WINDOW_WIDTH;

    vec2_t move_inputs = input_get_movement(window);
    vec2_t rotation_inputs = input_get_rotation(window);

    camera_update_transform_freecam(&camera, deltatime, move_inputs, rotation_inputs);

    glClearColor(0.1, 0.3f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void render_update_end() {
    glfwSwapBuffers(window);
    glfwPollEvents();
} 

void render_chunk(chunk_t* chunk) {
    glUseProgram(default_render_blocks[1].shader_id);
    mat4_t camera_mat = mat4_mul(camera.view, camera.proj);

    int x = 0;
    int z = 0;
    int start = -512;
    for (int i = 0; i < render_block_count; i++) {
        transform_t chunk_transform = {
            .position = {start + x * 32, -128.0f, start + z * 32 + 1},
            .rotation = {0.0f, 0.0f, 0.0f},
            .scale = {1.0f, 1.0f, 1.0f}
        };

        x++;
        if (x >= 32) {
            x = 0;
            z++;
        }

        glBindVertexArray(default_render_blocks[i].vao);

        glBindTexture(GL_TEXTURE_2D, default_render_blocks[i].texture_id);

        mat4_t test = mat4_mul(mat4_model(&chunk_transform), camera_mat);
        glUniformMatrix4fv(default_render_blocks[i].test_matrix_loc, 1, GL_FALSE, (float*)test.m);
        GLenum error = glGetError();

        glDrawElements(GL_TRIANGLES, base_chunks[i].index_count, GL_UNSIGNED_INT, 0);
    }

    glUseProgram(0);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        printf("OpenGL Error: 0x%x\n", error);
    }
}
