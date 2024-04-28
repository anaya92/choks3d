#version 410 core

#define MAX_TOTAL_LIGHTS 100
#define MAX_CLUSTERS 1000
#define MAX_CLUSTER_LIGHTS 50 

struct light_t
{
    int type;
    transform_t transform;
};

struct light_reference_t
{
    int offset;
    int count;
};

layout (std140) uniform lighting
{
    light_t global_light_list[MAX_TOTAL_LIGHTS];
    uint global_light_indices[MAX_CLUSTERS * MAX_CLUSTER_LIGHTS];
    light_reference_t light_references[MAX_CLUSTERS];
};

vec4 calculate_lighting_additive()
{

}