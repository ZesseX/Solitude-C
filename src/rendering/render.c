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

    render_init_block();
    
    return 1;
}

int renderer_update() {
    return 1;
}

void render_init_block() {
    int scs;
    default_render_block.shader_id = shader_load("shader/vert.vert", "shader/frag.frag", &scs);
    printf("Program id: %u\n", default_render_block.shader_id);

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

    GLuint vbo, vao, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
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
    glUniform1i(default_render_block.texture_loc, 0);

    default_render_block.texture_id;
    glGenTextures(1, &default_render_block.texture_id);
    glBindTexture(GL_TEXTURE_2D, default_render_block.texture_id);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    tga_image_t* texture = load_tga("textures/32x32.tga");

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->data);
    glGenerateMipmap(GL_TEXTURE_2D);

    camera.transform.position = (vec3_t){0.0f, 0.0f, -10.0f};
}

int first = 1;
void render_update_start() {
    float cur_frame_time = glfwGetTime();
    float deltatime = cur_frame_time - last_frame_time;
    last_frame_time = cur_frame_time;

    float aspect_ratio = (float)WINDOW_WIDTH / (float)WINDOW_WIDTH;
    cam_trans.position = (vec3_t){0.0f, 2.0f, -10.0f};
    //camera.transform.position = (vec3_t){0.0f, 2.0f, -10.0f};

    if (first) {
        vec3_print(camera.transform.position);
    }

    vec2_t move_inputs = input_get_movement(window);
    vec2_t rotation_inputs = input_get_rotation(window);

    camera_update_transform_freecam(&camera, deltatime, move_inputs, rotation_inputs);

    if (first) {
        vec3_print(camera.transform.position);
    }

    //camera_update_view_matrix(&camera, &cam_trans);
    //camera_update_proj_matrix(&camera, aspect_ratio);

    glClearColor(0.2, 0.6f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void render_update_end() {
    glfwSwapBuffers(window);
    glfwPollEvents();
} 

void render_chunk(chunk_t* chunk) {
    transform_t block_transform = {
        .position = {0.0f, -2.0f, 0.0f},
        .rotation = {0.0f, 0.0f, 0.0f},
        .scale = {1.0f, 1.0f, 1.0f}
    };

    glUseProgram(default_render_block.shader_id);
    glBindVertexArray(default_render_block.vao);

    glUniformMatrix4fv(default_render_block.matrix_view_loc, 1, GL_FALSE, (float*)&camera.view);
    glUniformMatrix4fv(default_render_block.matrix_projection_loc, 1, GL_FALSE, (float*)&camera.proj);
    
    for (int x = 0; x < 16; x++){
        for (int z = 0; z < 16; z++) {
            block_transform.position.x = x;
            //block_transform.position.y = 0;
            block_transform.position.z = z;

            glBindTexture(GL_TEXTURE_2D, default_render_block.texture_id);
            glUniformMatrix4fv(default_render_block.matrix_model_loc, 1, GL_FALSE, (float*)mat4_model(&block_transform).m);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }
    }

    if(first){
        mat4_print(mat4_model(&block_transform));

        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            printf("OpenGL Error: 0x%x\n", error);
        }
        first = 0;
    }
}
