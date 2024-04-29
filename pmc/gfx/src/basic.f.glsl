#version 400 core

out vec4 frag_out;

void main()
{
    // if (distance(gl_FragCoord.xyz / gl_FragCoord.w, pos) > 20.0f) discard;

    // if (distance(gl_PointCoord, vec2(0.5f, 0.5f)) > 0.5f) discard;

    frag_out = vec4(0.3f, 0.3f, 0.3f, 1.0f);
    // frag_out = vec4(mix(vec3(1.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), distance(gl_PointCoord, vec2(1, 0))), 1.0f);
}