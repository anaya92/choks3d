// turan_choks - OpenGL utility library
// edit choks_config.h for configuration.

// NOTE - higher level utility stuff was moved to upper_graphics

#pragma once

// CHOKS CONFIGURATION
#define CHOKS_DEBUG 1
#define CHOKS_WIDTH 1280
#define CHOKS_HEIGHT 800

#include <glad/gl.h>
#include "external/HandmadeMath.h"

// setup/cleanup
// -------------
extern void setup_choks();
extern void cleanup_choks();

// state machine :D
// ----------------

// THIS HAS TO BE IN ALL VERTEX SHADERS FOR CAM SUPPORT !!!!
// layout (std140) uniform mvp
// {
//     mat4 model;
//     mat4 view;
//     mat4 projection;
// };

extern void set_model_matrix(mat4_t model);
extern void set_view_and_projection_matrices(mat4_t view, mat4_t projection);

// PRIMITIVES
// ----------
typedef struct primitive_s
{
    unsigned int vao, vbo, ibo;
    int draw_mode;
    int vertex_count;
    int index_count;
} primitive_t;

typedef struct primitive_std_vertex_s
{
    float x, y, z;
    float s, t;
} primitive_std_vertex_t;

extern primitive_t primitive_load(float* data, int vertex_count, int draw_mode); // vertex: xyzst.
extern primitive_t primitive_load_with_indices(float* vertices, int vertex_count, unsigned int* indices, int index_count, int draw_mode);
extern void primitive_free(primitive_t* this);

extern void primitive_draw(primitive_t* this);

// PROGRAMS
// --------
typedef struct program_s
{
    unsigned int id;
} program_t;

extern program_t program_load_from_files(const char* vertex_shader_path, const char* fragment_shader_path);
extern program_t program_load_from_source(const char* vertex_source, const char* fragment_source);
extern program_t program_load_from_source_ex(const char* vertex_source, const char* fragment_source);
extern void program_free(program_t this);

// TEXTURES
// --------
// i think for this one ill be sticking to .webp
typedef enum
{
    CHOKS_TEXTURETYPE_2D,
    CHOKS_TEXTURETYPE_CUBEMAP,
} texturetype_t;

typedef struct
{
    texturetype_t type;
    unsigned int id;
    int width, height;
} texture_t;

extern texture_t texture_load_2d_from_mem(unsigned char* data);
extern texture_t texture_load_2d_from_file(const char* path);
extern texture_t texture_load_cubemap_from_file(const char* path);
extern void texture_free(texture_t this);