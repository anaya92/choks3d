#version 400 core

uniform sampler2D font;

uniform vec3 fgcolor;
uniform vec3 bgcolor;

uniform int index;

in vec2 st;

out vec4 frag_out;

void main()
{


    // frag_out = vec4(1.0, 0.0, 0.0, 1.0);

    float charwidth = 1.0 / 16;
    float charheight = 1.0 / 8;

    int x = index % 16;
    int y = (8 - 1) - (index / 16); // flip for opengl texture coordinates

    vec2 samplecoord;
    samplecoord.x = (charwidth * x) + (st.x * charwidth);
    samplecoord.y = (charheight * y) + (st.y * charheight);

    vec4 pixel = texture(font, samplecoord);
    if (pixel.r  < 0.1)
    {
        discard;
    }
    else frag_out = pixel * vec4(fgcolor, 1.0); 
}