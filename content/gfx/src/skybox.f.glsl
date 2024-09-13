#version 400 core

out vec4 frag_out;

in vec3 texture_coordinates;

uniform samplerCube skybox;

void main()
{    
    frag_out = texture(skybox, texture_coordinates);
}