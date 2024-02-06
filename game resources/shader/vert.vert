#version 430 core

layout (location = 0) in vec3 pos;

uniform mat4 mvp;

out vec2 tex_coords;

void main()
{
    gl_Position = mvp * vec4(pos, 1.0);

    // TODO: Fix the uvs for x & z axis facing facs
    tex_coords = pos.xz;
}