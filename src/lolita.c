#include "lolita.h"

#include <glad/gl.h>

#include "upper_graphics.h"

// software cluster handling
extern void software_generate_cluster_grid(camera_t* camera);
extern void software_populate_cluster_grid(camera_t* camera, light_t* lights, int light_count);

struct cluster_manager_s
{
    void (*generate_grid)(camera_t* camera);
    void (*populate_grid)();
    void (*update_buffer)(int id);
} clustermanager;

static int ubo = 0;

void setup_lolkim()
{
    // TODO: add hardware clustering support + detection of compute extensions
    clustermanager.generate_grid = software_generate_cluster_grid;
    clustermanager.populate_grid = software_populate_cluster_grid;

    // glGenBuffers(1, &ubo);
    // glBindBuffer(GL_UNIFORM_BUFFER, ubo);

    // glBufferData(GL_UNIFORM_BUFFER, sizeof())
}

void cleanup_lolkim()
{
    // glDeleteBuffers(1, &ubo);
}

void attach_lighting_data_to_program(program_t program)
{

}

void perftest(camera_t* cam)
{
    software_generate_cluster_grid(cam);
}