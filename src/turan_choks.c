#include "turan_choks.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define nil (void*)0

static struct choks_s
{
    struct choks_mvp_s
    {
        unsigned int ubo;

        struct choks_mvp_data_s
        {
            mat4_t model, view, proj;
        } data;
    } mvp;
    
} choks;

static void _dbgprintf(const char* file, const char* func, int line, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    printf("[%s | %s | ln%i] ", file, func, line);
    vprintf(fmt, args);
    va_end(args);    
}

#if CHOKS_DEBUG
#define choks_debug_printf(x, ...) _dbgprintf(__FILE__, __FUNCTION__, __LINE__, x __VA_OPT__(,) __VA_ARGS__)
#else
#define choks_debug_printf(x, ...)
#endif

// setup/cleanup
// -------------
void setup_choks()
{
    // initialize mvp ubo + data
    choks.mvp.data = (struct choks_mvp_data_s){
        HMM_Mat4d(1.0f),
        HMM_Mat4d(1.0f),
        HMM_Mat4d(1.0f),
    };

    glGenBuffers(1, &choks.mvp.ubo);

    glBindBuffer(GL_UNIFORM_BUFFER, choks.mvp.ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(struct choks_mvp_data_s), (void*) &choks.mvp.data, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, choks.mvp.ubo);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void cleanup_choks()
{
    glDeleteBuffers(1, &choks.mvp.ubo);
}


// state machine :D
// ----------------
// mvp:
void set_model_matrix(mat4_t model)
{
    glBindBuffer(GL_UNIFORM_BUFFER, choks.mvp.ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, (GLintptr) 0, sizeof(mat4_t), &model);
}

void set_view_and_projection_matrices(mat4_t view, mat4_t projection)
{
    glBindBuffer(GL_UNIFORM_BUFFER, choks.mvp.ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, (GLintptr) sizeof(mat4_t), sizeof(mat4_t), &view.elements[0][0]);
    glBufferSubData(GL_UNIFORM_BUFFER, (GLintptr) sizeof(mat4_t) * 2, sizeof(mat4_t), &projection.elements[0][0]);
}

// PRIMITIVES
// ----------
static void _setup_vao_attr()
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, nil);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, sizeof(float) * 5, (void*) (sizeof(float)*3));
    glEnableVertexAttribArray(1); 
}

primitive_t primitive_load(float* data, int vertex_count, int draw_mode) // vertex: xyzst.
{
    primitive_t this;    
    this.vertex_count = vertex_count;
    this.draw_mode = draw_mode;

    glGenVertexArrays(1, &this.vao);
    glBindVertexArray(this.vao);

    glGenBuffers(1, &this.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 5 * vertex_count, (const void*) data, GL_STATIC_DRAW);

    _setup_vao_attr();
    
    this.ibo = 0;

    return this;
}

primitive_t primitive_load_with_indices(float* vertices, int vertex_count, unsigned int* indices, int index_count, int draw_mode)
{
    primitive_t this;    
    this.vertex_count = vertex_count;
    this.index_count = index_count;
    this.draw_mode = draw_mode;

    glGenVertexArrays(1, &this.vao);
    glBindVertexArray(this.vao);

    glGenBuffers(1, &this.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 5 * vertex_count, (const void*) vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &this.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * this.index_count, indices, GL_STATIC_DRAW);

    _setup_vao_attr();

    return this;
}

void primitive_free(primitive_t* this)
{
    glDeleteBuffers(1, &this->ibo);
    glDeleteBuffers(1, &this->vbo);
    glDeleteVertexArrays(1, &this->vao);
}

// DEPRECATED ---------------------------------------------------------------------------------
#if 0
void primitive_attach_indices(primitive_t* this, unsigned int* data, int index_count) // unused
{
    this->index_count = index_count;

    glBindVertexArray(this->vao);
    glGenBuffers(1, &this->ibo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * this->vertex_count, data, GL_STATIC_DRAW);
}
#endif

void primitive_draw(primitive_t* this)
{
    glBindVertexArray(this->vao);

    if (this->ibo)
    {
        glDrawElements(this->draw_mode, this->index_count, GL_UNSIGNED_INT, nil);
        return;
    }

    glDrawArrays(this->draw_mode, 0, this->vertex_count);
}

// PROGRAMS
// --------
static char* slurp(const char* path) // ALL GOOD
{
    char* buffer = 0;
    size_t length;
    FILE* f = fopen(path, "r");

    if (f)
    {
        fseek(f, 0L, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(length);

        if (buffer)
        {
            size_t new_size = fread(buffer, 1, length, f);
            buffer[new_size] = '\0'; // FOR SAFETY
        }

        fclose(f);
    }

    return buffer;
}

static void validate_shader(int id)
{
    int successful;
    glGetShaderiv(id, GL_COMPILE_STATUS, &successful);
    if (!successful) {
        static char log[512];
        glGetShaderInfoLog(id, 512, NULL, log);
        printf("%s\n", log);
    }
}

program_t program_load_from_files(const char* vertex_shader_path, const char* fragment_shader_path)
{
    char* vertex_source = slurp(vertex_shader_path);
    char* fragment_source = slurp(fragment_shader_path);

    // printf("%s (vertex):\n%s\n%s (fragment):\n%s\n", vertex_shader_path, vertex_source, fragment_shader_path, fragment_source);

    if (!vertex_source || !fragment_source)
    {
        choks_debug_printf("program source paths not valid.\n");
        return (program_t) { 0 }; // invalid program
    }

    program_t this = program_load_from_source(vertex_source, fragment_source);

    free(vertex_source);
    free(fragment_source);

    return this;
}

program_t program_load_from_source(const char* vertex_source, const char* fragment_source)
{
    program_t this; // put a finger down if you can squirt.....
    this.id = glCreateProgram();

    // printf("vertex:\n%s\nfragment:\n%s\n", vertex_source, fragment_source);

    int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_source, nil);
    glCompileShader(vertex_shader);

    #if CHOKS_DEBUG
    choks_debug_printf("vert debug:\n");
    validate_shader(vertex_shader);
    #endif

    glAttachShader(this.id, vertex_shader);

    int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_source, nil);
    glCompileShader(fragment_shader);

    #if CHOKS_DEBUG
    choks_debug_printf("frag debug:\n");
    validate_shader(fragment_shader);
    #endif

    glAttachShader(this.id, fragment_shader);

    glLinkProgram(this.id);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    #if CHOKS_DEBUG
    int successful;
    glGetProgramiv(this.id, GL_LINK_STATUS, &successful);
    if (!successful) {
        static char log[512];
        glGetProgramInfoLog(this.id, 512, NULL, log);
        printf("%s\n", log);
    }
    choks_debug_printf("program success!!!\n");
    #endif

    // bind uniform block.
    glUniformBlockBinding(this.id, glGetUniformBlockIndex(this.id, "mvp"), 0);

    return this;
}

void program_free(program_t this)
{
    glDeleteProgram(this.id);
}

// TEXTURES
// --------
// i think for this one ill be sticking to .webp
#include <webp/decode.h>
#include <webp/demux.h>

static char* slurp_bytes(const char* path, size_t* size) // ALL GOOD (no mem err)
{
    char* buffer = 0;
    size_t length;
    FILE* f = fopen (path, "rb");

    if (f)
    {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(length);

        if (buffer)
        {
            size_t new_size = fread(buffer, 1, length, f);
            *size = new_size;
        }
        fclose (f);
    }

    return buffer;
}

texture_t texture_load_2d_from_file(const char* path)
{
    texture_t this = { 0 };

    size_t size;
    const uint8_t* data = (uint8_t*) slurp_bytes(path, &size);

    if (data)
    {
        int width, height;
        uint8_t* webp_data = WebPDecodeRGBA(data, size, &width, &height);

        if (!webp_data)
        {
            choks_debug_printf("failed to parse %s (not webp?)\n", path);
            free((void*) data);
            return this;
        }

        // A.O.K. proceed to load to gl
        glGenTextures(1, &this.id);
        glBindTexture(GL_TEXTURE_2D, this.id);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            width,
            height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            webp_data
        );

        // image configs TODO: make these texture filtering settings configurable etc. etc.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        WebPFree(webp_data);
    }
    else
    {
        choks_debug_printf("invalid file\n");
        return this;
    }

    free((void*)data);
    return this;
}


texture_t texture_load_cubemap_from_file(const char* path)
{
    texture_t this = { 0 };
    
    // load image
    size_t size;
    const uint8_t* data = (uint8_t*) slurp_bytes(path, &size);

    if (data)
    {
        choks_debug_printf("loaded file %s, size %i\n", path, size);

        WebPData webp_data = { data, size };

        WebPAnimDecoderOptions decoder_options;
        WebPAnimDecoderOptionsInit(&decoder_options);

        WebPAnimDecoder* decoder = WebPAnimDecoderNew(&webp_data, &decoder_options);

        int valid = 1;
    
        if (!decoder)
        {
            choks_debug_printf("animation decoder did not parse webp.\n");
            valid = 0;
        }

        WebPAnimInfo anim_info;
        WebPAnimDecoderGetInfo(decoder, &anim_info);

        // checks
        if (anim_info.frame_count != 6)
        {
            choks_debug_printf("cubemap webp does not have EXACTLY 6 frames.\n");
            valid = 0;
        }

        // after checks are done.
        if (valid)
        {
            // NOW generate opengl cubemap texture.
            glGenTextures(1, &this.id);
            glBindTexture(GL_TEXTURE_CUBE_MAP, this.id);

            #if CHOKS_DEBUG
            int index = 0;
            #endif
            while (WebPAnimDecoderHasMoreFrames(decoder))
            {
                uint8_t* buf;
                int timestamp;
                WebPAnimDecoderGetNext(decoder, &buf, &timestamp); // this buffer is rgba
                
                glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + index,
                    0,
                    GL_RGBA,
                    anim_info.canvas_width,
                    anim_info.canvas_height,
                    0,
                    GL_RGBA,
                    GL_UNSIGNED_BYTE,
                    buf
                );

                #if CHOKS_DEBUG
                index++;
                choks_debug_printf("loaded %i\n", index);
                #endif
            }

            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        }

        WebPAnimDecoderDelete(decoder);
    }
    else
    {
        choks_debug_printf("file not found!!!\n");
        return this;
    }

    done:
    free((void*) data);
    return this;
}

void texture_free(texture_t this)
{
    glDeleteTextures(1, &this.id);
}