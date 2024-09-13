#version 410 core

#define MAX_TOTAL_LIGHTS 100
#define MAX_CLUSTERS 1000
#define MAX_CLUSTER_LIGHTS 50 

struct light_t
{
    int type;
    vec4 position;
    float strength;
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

uint find_this_cluster(vec3 coordinates){
    return 0; // cluster index
}

vec4 calculate_lighting_additive()
{
    uint cluster = find_this_cluster(gl_FragCoord.xyz);

    for (int i = 0; i < light_references[cluster].count; i++)
    {
        // APPLY POINT LIGHT
        // +++++ surfaces should only be lit by a surface if that normal is facing towards this light
        //       aka how much a light affects a given surface should be dictated by the angle between
        //       the normal and the vector from the light to the vertex.
    }
}