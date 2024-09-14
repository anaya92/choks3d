// 2d sprites, basic font rendering.
// (0, 0) is top left of screen, works in
// pixel coordinates.
#pragma once

#include "turan_choks.h"

extern void ren2d_init();
extern void ren2d_cleanup();

typedef struct
{
    unsigned int id;
    int width, height;  
} sprite_t;

extern void sprite_load_from_file(const char* path);
extern void sprite_unload(sprite_t* spr);

extern void sprite_draw(sprite_t* spr, int x, int y);
extern void sprite_draw_ex(sprite_t* spr, int x, int y);

// temporary font drawing shitz
typedef struct
{
    texture_t texture;

    int charwidth, charheight;
} spritefont_t;

extern spritefont_t spritefont_load_from_img(const char* path);
extern void spritefont_free(spritefont_t* this);

extern void draw_text_spritefont(spritefont_t* font, float scale, vec3_t rgb, const char* text, vec2_t pos);