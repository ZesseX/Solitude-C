#version 430 core
out vec4 FragColor;

in vec2 tex_coords;

uniform sampler2D texture_sampler;

void main()
{
    FragColor = texture(texture_sampler, tex_coords);
}