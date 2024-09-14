#version 400 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texture;

out vec2 st;

void main()
{
    st = texture;
    gl_Position = vec4(position.xy, 0.0, 1.0);
}
