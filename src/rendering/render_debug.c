#include "render_debug.h"

static int ogl_debug_flags = 0;

void opengl_debug_init(const int flags) {
    ogl_debug_flags = flags;
}

void APIENTRY opengl_debug_msg_callback(
    GLenum src, GLenum type, GLuint id, GLenum severity,
    GLsizei length, const GLchar* msg, const void* data) {
    
    char* src_text;
    char* type_text;
    char* severity_text;

    if (severity = GL_DEBUG_SEVERITY_NOTIFICATION) {
        if (!(ogl_debug_flags & OGL_DEBUG_FLAG_NOTIFICATION)) return;
        severity_text = "NOTIFICATION";
    }
    else if (severity = GL_DEBUG_SEVERITY_LOW) {
        if (!(ogl_debug_flags & GL_DEBUG_SEVERITY_LOW)) return;
        severity_text = "LOW";
    }
    else if (severity = GL_DEBUG_SEVERITY_MEDIUM) {
        if (!(ogl_debug_flags & GL_DEBUG_SEVERITY_MEDIUM)) return;
        severity_text = "MEDIUM";
    }
    else if (severity = GL_DEBUG_SEVERITY_HIGH) {
        if (!(ogl_debug_flags & GL_DEBUG_SEVERITY_HIGH)) return;
        severity_text = "HIGH";
    }
    else {
        severity_text = "UNKNOWN";
    }

    switch (src) {
        case GL_DEBUG_SOURCE_API:
        src_text = "API";
        break;

        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        src_text = "WINDOW SYSTEM";
        break;

        case GL_DEBUG_SOURCE_SHADER_COMPILER:
        src_text = "SHADER COMPILER";
        break;

        case GL_DEBUG_SOURCE_THIRD_PARTY:
        src_text = "THIRD PARTY";
        break;

        case GL_DEBUG_SOURCE_APPLICATION:
        src_text = "APPLICATION";
        break;

        case GL_DEBUG_SOURCE_OTHER:
        src_text = "UNKNOWN";
        break;

        default:
        src_text = "UNKNOWN";
        break;
    }

    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
        type_text = "ERROR";
        break;

        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        type_text = "DEPRECATED BEHAVIOR";
        break;

        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        type_text = "UDEFINED BEHAVIOR";
        break;

        case GL_DEBUG_TYPE_PORTABILITY:
        type_text = "PORTABILITY";
        break;

        case GL_DEBUG_TYPE_PERFORMANCE:
        type_text = "PERFORMANCE";
        break;

        case GL_DEBUG_TYPE_OTHER:
        type_text = "OTHER";
        break;

        case GL_DEBUG_TYPE_MARKER:
        type_text = "MARKER";
        break;

        default:
        type_text = "UNKNOWN";
        break;
    }

    printf("[OpenGL] %d: %s of %s severity from %s: %s\n", id, type_text, severity_text, src_text, msg);
}