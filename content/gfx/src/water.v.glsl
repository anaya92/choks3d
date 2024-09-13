#version 400 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture;

layout (std140) uniform mvp
{
    mat4 model;
    mat4 view;
    mat4 projection;
};

out vec2 st;

void main()
{
    st = texture;
    gl_Position = projection * view * model * vec4(position, 1.0);
}