#version 400 core

out vec4 frag_out;

in vec2 st;

uniform sampler2D texture0;

void main()
{
    frag_out = texture(texture0, st);
}