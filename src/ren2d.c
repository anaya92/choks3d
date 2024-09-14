#include "ren2d.h"

#include "turan_choks.h"
#include "external/HandmadeMath.h"

#include <stdio.h>

static mat4_t projection;

// the quad to be used for all 2d rendering purposes
static struct
{
    unsigned int vao;
    unsigned int vbo;
    unsigned int ibo;
} _quaddata;

// spritefont rendering data
static struct 
{
    program_t shader;
    struct
    {
        int model;

        int fg;
        int bg;

        int charindex;
        int charwidth;
        int charheight;
    } uniforms;
} sfrenderer;

void ren2d_init()
{
    // setup projection according to screen dimensions
    projection = HMM_Orthographic(0.0f, CHOKS_WIDTH, CHOKS_HEIGHT, 0.0f, -1.0f, 1.0f);

    // setup the one quad we need for all this
    static float quadcoords[] = {
        0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f
    };

    static unsigned int quadindices[] = {
        0, 1, 2, 
        2, 0, 3
    };

    glGenVertexArrays(1, &_quaddata.vao);
    glBindVertexArray(_quaddata.vao);

    glGenBuffers(1, &_quaddata.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _quaddata.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadcoords), quadcoords, GL_STATIC_DRAW);

    glGenBuffers(1, &_quaddata.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _quaddata.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadindices), quadindices, GL_STATIC_DRAW);

    // pos.xy = xy, pos.zw = st
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*) 0);
    glEnableVertexAttribArray(0);

    // load the shaders and stuff we need

    // SETUP SPRITEFONT RENDERER
    sfrenderer.shader = program_load_from_files("gfx/src/bitmapfont.v.glsl", "gfx/src/bitmapfont.f.glsl");
    glUseProgram(sfrenderer.shader.id);

    glUniformMatrix4fv(glGetUniformLocation(sfrenderer.shader.id, "projection"), 1, GL_FALSE, &projection.elements[0][0]);
    sfrenderer.uniforms.fg = glGetUniformLocation(sfrenderer.shader.id, "fgcolor");
    sfrenderer.uniforms.charindex = glGetUniformLocation(sfrenderer.shader.id, "index");
    sfrenderer.uniforms.model = glGetUniformLocation(sfrenderer.shader.id, "model");
}

void ren2d_cleanup()
{
    // CLEANUP SPRITEFONT RENDERER
    program_free(sfrenderer.shader);

    // cleanup quad buffer
    glDeleteBuffers(1, &_quaddata.ibo);
    glDeleteBuffers(1, &_quaddata.vbo);
    glDeleteVertexArrays(1, &_quaddata.vao);
}

spritefont_t spritefont_load_from_img(const char* path)
{
    spritefont_t font = { 0 };
    font.texture = texture_load_2d_from_file(path);

    if (!font.texture.id) printf("texture malfunction\n");

    // TEMP: 16x8 fixed font sheet data
    // this can have a variable resolution,
    // but there is an expected format to be
    // followed.
    font.charwidth = font.texture.width / 16;
    font.charheight = font.texture.width / 8;
    printf("%i %i\n", font.charwidth, font.charheight);

    return font;
}

void spritefont_free(spritefont_t* this)
{
    texture_free(this->texture);
}

// TODO: batched rendering for fonts + other optimizations
void draw_text_spritefont(spritefont_t* font, float scale, vec3_t rgb, const char* text, vec2_t pos)
{
    glUseProgram(sfrenderer.shader.id);
    glUniform3fv(sfrenderer.uniforms.fg, 1, rgb.elements);

    mat4_t model = HMM_Mat4d(1.0f);
    model = HMM_MultiplyMat4(HMM_Scale((vec3_t){scale * font->charwidth, scale * font->charheight, 1.0f}), model);
    model = HMM_MultiplyMat4(HMM_Translate((vec3_t) { pos.x, pos.y, 0.0f}), model);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, font->texture.id);
    glBindVertexArray(_quaddata.vao);

    int i = 0;
    while (text[i] != '\0')
    {
        // TODO: overflow handling
        int index = text[i];
        index %= 128;        

        glUniform1i(sfrenderer.uniforms.charindex, index);

        glUniformMatrix4fv(sfrenderer.uniforms.model, 1, GL_FALSE, &model.elements[0][0]);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

        model = HMM_MultiplyMat4(HMM_Translate((vec3_t) { (float) font->charwidth * scale, 0.0f, 0.0f}), model);

        i++;
    }
}