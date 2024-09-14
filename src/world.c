#include "world.h"

#include "turan_choks.h"

// temp primitive data (x, y, z, u, v)
static float tempworlddata[] = {
    -5.0f, 0.0f, 5.0f, 0.0f, 0.0f,
    -5.0f, -0.0f, -5.0f, 0.0f, 25.0f,
    5.0f, -0.0f, 5.0f, 25.0f, 0.0f,
    5.0f, -0.0f, -5.0f, 25.0f, 25.0f,
};

static unsigned int tempworldindicies[] = {
    0, 1, 2,
    2, 1, 3
};

static primitive_t terrain_mesh;
static program_t basic_program;
static texture_t tiles_texture;

void world_generate_test()
{
    terrain_mesh = primitive_load_with_indices(tempworlddata, 4, tempworldindicies, 6, GL_TRIANGLES);
    basic_program = program_load_from_files("gfx/src/textured.v.glsl", "gfx/src/textured.f.glsl");
    tiles_texture = texture_load_2d_from_file("media/misc/tiles.webp");
}

void world_cleanup()
{
    texture_free(tiles_texture);
    program_free(basic_program);
    primitive_free(&terrain_mesh);
}

void world_draw()
{
    glBindTexture(GL_TEXTURE_2D, tiles_texture.id);
    glUseProgram(basic_program.id);
    primitive_draw(&terrain_mesh);
}