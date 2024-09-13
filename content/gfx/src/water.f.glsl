#version 400 core

uniform float time;
uniform sampler2D scrolling;

in vec2 st;

out vec4 frag_out;

void main()
{
    float offset = time / 25;
    vec2 scroll_right = st + vec2(offset, -offset);
    vec2 scroll_left = (vec2(1.0) - st) + vec2(offset, offset);

    vec4 mixed_colors = mix(texture(scrolling, scroll_left), texture(scrolling, scroll_right), 0.5);
    
    if (0.25 < mixed_colors.r && mixed_colors.r < 0.4)
    {
        // frag_out = mix(vec4(vec3(1.0), 1.0 - mixed_colors.r), vec4(0.18, 0.44, 0.38, 1.0 - mixed_colors.r), 0.25);
        discard;
    }
    else frag_out = vec4(vec3(1.0), 0.5 - mixed_colors.r);

    // frag_out = mixed_colors;
}