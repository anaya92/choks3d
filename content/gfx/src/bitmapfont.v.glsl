#version 400 core

layout (location = 0) in vec4 position;

uniform mat4 model;
uniform mat4 projection;

out vec2 st;

void main()
{
    st = position.zw;
    gl_Position = projection * model * vec4(position.xy, 0.0, 1.0);
}
