#version 400 core
layout (location = 0) in vec3 position;

out vec3 texture_coordinates;

layout (std140) uniform mvp
{
    mat4 model;
    mat4 view;
    mat4 projection;
};

void main()
{
    texture_coordinates = vec3(position.xy, -position.z);
    vec4 pos = projection * mat4(mat3(view)) * vec4(position, 1.0);
    gl_Position = pos.xyww;
}  
