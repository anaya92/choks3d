// upper graphics are for renderer tooling which is not as low level
// as what is done in turan_choks - meshes, camera, etc go here.

#pragma once

#include "turan_choks.h"
#include "external/HandmadeMath.h"

// little transform/camera utilities - moved from turan_choks
// ----------------------------------------------------------
typedef struct transform_s
{
    vec3_t position;
    vec3_t rotate;
    vec3_t scale;
} transform_t;

extern mat4_t transform_to_matrix(transform_t* this);

// CAMERA CAMERA CAMERA !!
// -----------------------
typedef struct camera_s
{
    transform_t transform;

    float fov;
    float aspect;
    float near;
    float far;

    // useful vectors
    vec3_t front;

    // matrices (for free access here)
    struct
    {
        mat4_t view, projection;
    } matrices;
} camera_t;

extern void camera_update_view(camera_t* this); // for every frame/when u change transform
extern void camera_update_projection(camera_t* this); // only when you need to/when u change frustrum/viewport