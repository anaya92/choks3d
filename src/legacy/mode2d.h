// so all 2d things are drawn on quads, from textures to fonts to whatever.
// (0, 0) is the top left corner
#pragma once

extern void init_mode2d();
extern void cleanup_mode2d();

extern void draw_texture_2d();
extern void draw_texture_2d_ex();

extern void draw_quad_rgba();