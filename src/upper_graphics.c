#include "upper_graphics.h"

// little transform/camera utilities - moved from turan_choks.c
// ------------------------------------------------------------
mat4_t transform_to_matrix(transform_t* this)
{
    mat4_t matrix = HMM_Mat4d(1.0f);

    // ORDER MATTERS! S * R * T
    matrix = HMM_MultiplyMat4(HMM_Scale(this->scale), matrix);

    this->rotate.x = fmodf(this->rotate.x, 360.0f);
    this->rotate.y = fmodf(this->rotate.y, 360.0f);
    this->rotate.z = fmodf(this->rotate.z, 360.0f); 

    matrix = HMM_MultiplyMat4(HMM_Rotate(this->rotate.x, HMM_Vec3(1.0f, 0.0f, 0.0f)), matrix);
    matrix = HMM_MultiplyMat4(HMM_Rotate(this->rotate.y, HMM_Vec3(0.0f, 1.0f, 0.0f)), matrix);
    matrix = HMM_MultiplyMat4(HMM_Rotate(this->rotate.z, HMM_Vec3(0.0f, 0.0f, 1.0f)), matrix);

    matrix = HMM_MultiplyMat4(HMM_Translate(this->position), matrix);

    return matrix;
}

// CAMERA CAMERA CAMERA !!
// -----------------------
void camera_update_view(camera_t* this)
{
    vec3_t direction = { 0.0f };

    direction.x = HMM_CosF(HMM_ToRadians(this->transform.rotate.y)) * HMM_CosF(HMM_ToRadians(this->transform.rotate.x));
    direction.y = HMM_SinF(HMM_ToRadians(this->transform.rotate.x));
    direction.z = HMM_SinF(HMM_ToRadians(this->transform.rotate.y)) * HMM_CosF(HMM_ToRadians(this->transform.rotate.x));
    
    this->front = HMM_NormalizeVec3(direction);

    static vec3_t up = (vec3_t) { 0.0f, 1.0f, 0.0f }; // TODO: IMPLEMENT CAMERA ROLLING.

    this->matrices.view = HMM_LookAt(this->transform.position, HMM_AddVec3(this->transform.position, this->front), up); // rotate some of this stuff
    
}

void camera_update_projection(camera_t* this)
{
    this->matrices.projection = HMM_Perspective(this->fov / 2, this->aspect, this->near, this->far);
}