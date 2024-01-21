#include "shader.h"

shader_t create_shader_program(const char* vert_path, const char* frag_path, int* success) {
    shader_t shader;

    int scs;
    shader.id = shader_load(vert_path, frag_path, &scs);

    *success = scs;
    return shader;
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
    char info_log[512];

    glGetShaderiv(vert_id, GL_COMPILE_STATUS, &scs);
    if (!scs){
        glGetShaderInfoLog(vert_id, 512, NULL, info_log);
        fprintf(stderr, "Failed to compile vertex shader (id: %u):\n %s\n", vert_id, info_log);
        return 0;
    }

    glShaderSource(frag_id, 1, &frag_code, NULL);
    glCompileShader(frag_id);

    glGetShaderiv(frag_id, GL_COMPILE_STATUS, &scs);
    if (!scs){
        glGetShaderInfoLog(frag_id, 512, NULL, info_log);
        fprintf(stderr, "Failed to compile vertex shader (id: %u):\n %s\n", frag_id, info_log);
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
