#include "render.h"

GLFWwindow* window = NULL;

int renderer_init(){
    if (!glfwInit()) {
        printf("Failed to initialize glfw\n");
        return 0;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 600, "Solitude", NULL, NULL);
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

    render_block();

    glfwTerminate();
    return 1;
}

int renderer_update() {
    //render_block();
    return 1;
}

/*
* TEMPORARY STUFF
*/

typedef struct {
    mat4_t view;
    mat4_t proj;
} camera_t;

camera_t camera;

void update_view_matrix(camera_t* camera, transform_t* t){
    camera->view = mat4_look_at(t->position, (vec3_t){0.0f, 0.0f, 0.0f}, (vec3_t){0.0f, 1.0f, 0.0f});
}

void update_projection_matrix(camera_t* camera, float aspect_ratio){
    camera->proj = mat4_perspective(45.0f, aspect_ratio, 0.1f, 100.0f);
}

/*
* END
*/

void render_block(){
    int scs;
    GLuint program_id = shader_load("shader/vert.vert", "shader/frag.frag", &scs);
    printf("Program id: %u\n", program_id);

    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f,  // top left 
        
        // other side
         0.5f,  0.5f, 1.0f,  // top right
         0.5f, -0.5f, 1.0f,  // bottom right
        -0.5f, -0.5f, 1.0f,  // bottom left
        -0.5f,  0.5f, 1.0f,  // top left 
    };

    unsigned int indices[] = { 
        0, 1, 3,  // first Triangle
        1, 2, 3,

        4, 5, 0,
        5, 1, 0,

        7, 6, 4,
        6, 5, 4
    };

    unsigned int vbo, vao, ebo;
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

    transform_t cube_trans = {
        .position = {0.0f, 1.0f, 0.0f},
        .rotation = {0.0f, 0.8f, 0.0f},
        .scale = {2.0f, 1.0f, 1.0f}
    };

    transform_t cam_trans = {
        .position = {0.0f, 0.0f, -5.0f},
        .rotation = {0.0f, 0.0f, 0.0f},
        .scale = {1.0f, 1.0f, 1.0f}
    };

    float aspect_ratio = 800.0f / 600.0f;
    update_view_matrix(&camera, &cam_trans);
    update_projection_matrix(&camera, aspect_ratio);

    glUseProgram(program_id);
    GLuint viewLoc = glGetUniformLocation(program_id, "view");
    GLuint projLoc = glGetUniformLocation(program_id, "projection");
    GLuint modeLoc = glGetUniformLocation(program_id, "model");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float*)&camera.view);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, (float*)&camera.proj);
    glUniformMatrix4fv(modeLoc, 1, GL_FALSE, (float*)mat4_model(&cube_trans).m);

    while(!glfwWindowShouldClose(window)){
        glClearColor(0.1, 0.6f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program_id);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

char* shader_load_file(const char* path) {
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        fprintf(stderr, "Failed to open shader file: %s\n", path);
        return NULL;
    }
   
    fseek(file, 0L, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(size + 1);
    if (!buffer) {
        fprintf(stderr, "Failed to allocate memory for shader file: %s\n", path);
        fclose(file);
        return NULL;
    }

    size_t read_size = fread(buffer, 1, size, file);
    buffer[read_size] = '\0';

    fclose(file);

    return buffer;
}

GLuint shader_load(const char* vert_path, const char* frag_path, int* success)
{
    *success = 0;

    GLuint vert_id = glCreateShader(GL_VERTEX_SHADER);
    GLuint frag_id = glCreateShader(GL_FRAGMENT_SHADER);

    const char* vert_code = shader_load_file(vert_path);
    const char* frag_code = shader_load_file(frag_path);

    glShaderSource(vert_id, 1, &vert_code, NULL);
    glCompileShader(vert_id);

    int scs;
    glGetShaderiv(vert_id, GL_COMPILE_STATUS, &scs);
    if (!scs){
        fprintf(stderr, "Failed to compile vertex shader (id: %u)\n", vert_id);
        return 0;
    }

    glShaderSource(frag_id, 1, &frag_code, NULL);
    glCompileShader(frag_id);

    glGetShaderiv(frag_id, GL_COMPILE_STATUS, &scs);
    if (!scs){
        fprintf(stderr, "Failed to compile vertex shader (id: %u)\n", frag_id);
        return 0;
    }

    GLuint program_id = glCreateProgram();
    glAttachShader(program_id, vert_id);
    glAttachShader(program_id, frag_id);
    glLinkProgram(program_id);

    glGetProgramiv(program_id, GL_LINK_STATUS, &scs);
    if (!scs){
        fprintf(stderr, "Failed to create shader program (id: %u)\n", program_id);
        return 0;
    }
    glDeleteShader(vert_id);
    glDeleteShader(frag_id);

    *success = 1;
    return program_id;
}
