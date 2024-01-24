#include "render.h"

GLFWwindow* window = NULL;

transform_t cam_trans;
camera_t camera;
render_block_t default_render_block;

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
    glfwSwapInterval(1);

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

chunk_t temp_chunk;
int renderer_update() {
    render_update_start();
    render_chunk(&temp_chunk);
    render_update_end();

    return glfwWindowShouldClose(window);
}

void render_init_temp_chunk() {
    int scs;
    default_render_block.shader_id = shader_load("shader/vert.vert", "shader/frag.frag", &scs);
    printf("Program id: %u\n", default_render_block.shader_id);

    /*
    float vertices[] = {
        -0.5, -0.5,  0.5,
         0.5, -0.5,  0.5,
         0.5,  0.5,  0.5,
        -0.5,  0.5,  0.5,

        -0.5, -0.5, -0.5,
         0.5, -0.5, -0.5,
         0.5,  0.5, -0.5,
        -0.5,  0.5, -0.5
    };
    unsigned int indices[] = { 
        0, 1, 2,
        2, 3, 0,
        
        1, 5, 6,
        6, 2, 1,
        
        7, 6, 5,
        5, 4, 7,
        
        4, 0, 3,
        3, 7, 4,
        
        4, 5, 1,
        1, 0, 4,
        
        3, 2, 6,
        6, 7, 3
    };
    */

    init_chunk(&temp_chunk);
    
    int vertex_count;
    GLfloat* vertices = get_chunk_vertices(&vertex_count);

    GLuint* indices = get_chunk_block_indices(&temp_chunk);

    GLuint vbo, vao, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertex_count, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * temp_chunk.index_count, indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    default_render_block.vao = vao;
    default_render_block.vbo = vbo;
    default_render_block.ebo = ebo;

    glUseProgram(default_render_block.shader_id);
    default_render_block.matrix_view_loc = glGetUniformLocation(default_render_block.shader_id, "view");
    default_render_block.matrix_projection_loc = glGetUniformLocation(default_render_block.shader_id, "projection");
    default_render_block.matrix_model_loc = glGetUniformLocation(default_render_block.shader_id, "model");

    default_render_block.texture_loc = glGetUniformLocation(default_render_block.shader_id, "texture_sampler");
    texture_t* texture = load_texture("textures/32x32.tga");
    default_render_block.texture_id = texture->id;

    camera.transform.position = (vec3_t){0.0f, 0.0f, -10.0f};
}

void render_update_start() {
    float cur_frame_time = glfwGetTime();
    float deltatime = cur_frame_time - last_frame_time;
    last_frame_time = cur_frame_time;

    float aspect_ratio = (float)WINDOW_WIDTH / (float)WINDOW_WIDTH;

    vec2_t move_inputs = input_get_movement(window);
    vec2_t rotation_inputs = input_get_rotation(window);

    camera_update_transform_freecam(&camera, deltatime, move_inputs, rotation_inputs);

    //vec3_print(camera.transform.rotation);

    glClearColor(0.1, 0.3f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void render_update_end() {
    glfwSwapBuffers(window);
    glfwPollEvents();
} 

void render_chunk(chunk_t* chunk) {
    transform_t block_transform = {
        .position = {0.0f, 1.0f, 0.0f},
        .rotation = {0.0f, 0.0f, 0.0f},
        .scale = {1.0f, 1.0f, 1.0f}
    };

    glUseProgram(default_render_block.shader_id);
    glBindVertexArray(default_render_block.vao);

    glUniformMatrix4fv(default_render_block.matrix_view_loc, 1, GL_FALSE, (float*)&camera.view);
    glUniformMatrix4fv(default_render_block.matrix_projection_loc, 1, GL_FALSE, (float*)&camera.proj);

    glBindTexture(GL_TEXTURE_2D, default_render_block.texture_id);
    glUniformMatrix4fv(default_render_block.matrix_model_loc, 1, GL_FALSE, (float*)mat4_model(&block_transform).m);
    
    glDrawElements(GL_TRIANGLES, temp_chunk.index_count, GL_UNSIGNED_INT, 0);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        printf("OpenGL Error: 0x%x\n", error);
    }
}
