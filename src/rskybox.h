#pragma once

#include "turan_choks.h"

static struct
{
    unsigned int vao, vbo;

    program_t shaderprog;
} _rskybox;

static float _rskybox_vertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

static void rskybox_setup()
{
    glGenVertexArrays(1, &_rskybox.vao);
    glBindVertexArray(_rskybox.vao);

    glGenBuffers(1, &_rskybox.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _rskybox.vbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), _rskybox_vertices);
    glEnableVertexAttribArray(0);

    _rskybox.shaderprog = program_load_from_files("gfx/src/skybox.v.glsl", "gfx/src/skybox.f.glsl");
}

static void rskybox_cleanup()
{
    glDeleteBuffers(1, &_rskybox.vbo);
    glDeleteVertexArrays(1, &_rskybox.vao);

    program_free(_rskybox.shaderprog);
}

static void rskybox_render(texture_t tex)
{
    glDepthFunc(GL_LEQUAL);
    glUseProgram(_rskybox.shaderprog.id);
    glBindVertexArray(_rskybox.vao);
    glBindTexture(GL_TEXTURE_CUBE_MAP, tex.id);  		
    glDrawArrays(GL_TRIANGLES, 0, 36);	 
    glDepthFunc(GL_LESS);
}