// holly kim (lolita) - clustered lighting
#pragma once

#include "turan_choks.h"
#include "upper_graphics.h"

// KIM CONFIGURATION
#define KIM_LIGHTS_PER_CALL 4

// the type.
typedef struct
{
    vec4_t position;
    int type;
    float strength;
} light_t; // point light only rn.

extern void setup_lolkim();
extern void cleanup_lolkim();

extern void attach_lighting_data_to_program(program_t program);

extern void update_lighting_clusters(camera_t* camera);

extern void perftest(camera_t* cam);