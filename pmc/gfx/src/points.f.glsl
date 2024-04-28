#version 400 core

out vec4 frag_out;

void main()
{
    if (distance(gl_PointCoord, vec2(0.5f, 0.5f)) > 0.5f) discard;
    frag_out = vec4(mix(vec3(0.75f, 0.13f, 0.12f), vec3(0.0f, 0.0f, 0.0f), distance(gl_PointCoord, vec2(1, 0))), 1.0f);
} 