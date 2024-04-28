// software lighting clustering algo - anaya khadka (c)
// http://www.aortiz.me/2018/12/21/CG.html

// this is for sub gl 4.3 compatible gpus where we cannot
// take advantage of the compute shader functionality.

// rendering pipeline:
// 1. build cluster grid
// 2. pre-pass into depth buffer
// 3. find visible clusters
// 4. reduce repeated instances
// 5. light culling + cluster assignment
// 6. shade

#include "turan_choks.h"
#include "upper_graphics.h"

#include "lolita.h"

#include <pthread.h>
#include <stdio.h>
#include <math.h>

/* CONFIGURATION */
#define MAX_LIGHTS_IN_CLUSTER 50
#define DEPTH_SLICE_COUNT 10
#define HORIZONTAL_SLICE_COUNT 10
#define VERTICAL_SLICE_COUNT 10
/* ============= */
/* = CONSTANTS = */
#define TOTAL_CLUSTER_COUNT HORIZONTAL_SLICE_COUNT * VERTICAL_SLICE_COUNT * DEPTH_SLICE_COUNT
#define CLUSTER_GRID_DIMENSIONS (vec3_t) { HORIZONTAL_SLICE_COUNT, VERTICAL_SLICE_COUNT, DEPTH_SLICE_COUNT, }
/* CPU OPTIMIZ.. */
// TODO: add checks here or smth
#ifdef __SSE__

#include <xmmintrin.h>

static vec4_t _fast_vec4_min(vec4_t one, vec4_t two)
{
    return (vec4_t) { ._internal_sse = _mm_min_ps(one._internal_sse, two._internal_sse) };
}

static vec4_t _fast_vec4_max(vec4_t one, vec4_t two)
{
    return (vec4_t) { ._internal_sse = _mm_max_ps(one._internal_sse, two._internal_sse) };
}
#endif
/* ============= */

static vec4_t m128_to_vec4(__m128 val)
{
    return (vec4_t) { ._internal_sse = val };
}

// taken from handmade math 2.0
static mat4_t _mat4_inverse(mat4_t in)
{
    vec3_t c01 = HMM_Cross(m128_to_vec4(in.columns[0]).xyz, m128_to_vec4(in.columns[1]).xyz);
    vec3_t c23 = HMM_Cross(m128_to_vec4(in.columns[2]).xyz, m128_to_vec4(in.columns[3]).xyz);
    vec3_t b10 = HMM_SubtractVec3(HMM_MultiplyVec3f(m128_to_vec4(in.columns[0]).xyz, m128_to_vec4(in.columns[1]).w), HMM_MultiplyVec3f(m128_to_vec4(in.columns[1]).xyz, m128_to_vec4(in.columns[0]).w));
    vec3_t b32 = HMM_SubtractVec3(HMM_MultiplyVec3f(m128_to_vec4(in.columns[2]).xyz, m128_to_vec4(in.columns[3]).w), HMM_MultiplyVec3f(m128_to_vec4(in.columns[3]).xyz, m128_to_vec4(in.columns[2]).w));

    float inv_determinant = 1.0f / (HMM_DotVec3(c01, b32) + HMM_DotVec3(c23, b10));
    c01 = HMM_MultiplyVec3f(c01, inv_determinant);
    c23 = HMM_MultiplyVec3f(c23, inv_determinant);
    b10 = HMM_MultiplyVec3f(b10, inv_determinant);
    b32 = HMM_MultiplyVec3f(b32, inv_determinant);

    mat4_t result;
    result.columns[0] = HMM_Vec4v(HMM_AddVec3(HMM_Cross(m128_to_vec4(in.columns[1]).xyz, b32), HMM_MultiplyVec3f(c23, m128_to_vec4(in.columns[1]).w)), -HMM_DotVec3(m128_to_vec4(in.columns[1]).xyz, c23))._internal_sse;
    result.columns[1] = HMM_Vec4v(HMM_SubtractVec3(HMM_Cross(b32, m128_to_vec4(in.columns[0]).xyz), HMM_MultiplyVec3f(c23, m128_to_vec4(in.columns[0]).w)), +HMM_DotVec3(m128_to_vec4(in.columns[0]).xyz, c23))._internal_sse;
    result.columns[2] = HMM_Vec4v(HMM_AddVec3(HMM_Cross(m128_to_vec4(in.columns[3]).xyz, b10), HMM_MultiplyVec3f(c01, m128_to_vec4(in.columns[3]).w)), -HMM_DotVec3(m128_to_vec4(in.columns[3]).xyz, c01))._internal_sse;
    result.columns[3] = HMM_Vec4v(HMM_SubtractVec3(HMM_Cross(b10, m128_to_vec4(in.columns[2]).xyz), HMM_MultiplyVec3f(c01, m128_to_vec4(in.columns[2]).w)), +HMM_DotVec3(m128_to_vec4(in.columns[2]).xyz, c01))._internal_sse;

    return HMM_Transpose(result); 
}

typedef struct cluster_s
{
    vec4_t min;
    vec4_t max;
} cluster_t;

typedef struct cluster_light_reference_s
{
    int offset;
    int length;
} cluster_light_reference_t;

// globals
static cluster_t cluster_grid[TOTAL_CLUSTER_COUNT];
static cluster_light_reference_t cluster_lights[TOTAL_CLUSTER_COUNT];

static unsigned int global_light_index_list[TOTAL_CLUSTER_COUNT * MAX_LIGHTS_IN_CLUSTER];
static int global_light_index_count = 0;

static int screen_tile_size = CHOKS_WIDTH / 10;

void setup_software_clustering(int width, int height)
{
}

void cleanup_software_clustering()
{
}

float _get_slice_from_depth(camera_t* cam, float depth)
{
    float scale = logf(depth) * (DEPTH_SLICE_COUNT / logf(cam->far / cam->near));
    float bias = (DEPTH_SLICE_COUNT * logf(cam->near)) / logf(cam->far / cam->near);

    return scale - bias;
}

vec4_t _screenspace_to_viewspace(camera_t* cam, mat4_t inv_proj, vec4_t pos)
{
    vec2_t st = HMM_DivideVec2(pos.xy, (vec2_t) { CHOKS_WIDTH, CHOKS_HEIGHT });
    
    vec2_t clip_xy = HMM_SubtractVec2(HMM_MultiplyVec2f(HMM_Vec2(st.x, 1.0f - st.y), 2.0), (vec2_t) { 2.0f });
    vec4_t clip = HMM_Vec4(
        clip_xy.x,
        clip_xy.y,
        pos.z,
        pos.w
    );

    mat4_t inverse_projection; // TODO: who fucking knows how to figure this out..
    vec4_t viewspace_pos = HMM_MultiplyMat4ByVec4(inverse_projection, clip);

    viewspace_pos = HMM_DivideVec4f(viewspace_pos, viewspace_pos.w);
    
    return viewspace_pos;
}

static vec3_t _index_to_xyz(int index, vec3_t max)
{
    vec3_t xyz;

    xyz.x = (float) (index % (int) max.x);
    xyz.y = (float) ((index / (int) max.x) % (int) max.y);
    xyz.z = (float) (index / ((int) max.x * (int) max.y));

    return xyz;
}

static int _xyz_to_index(vec3_t xyz, vec3_t max) // just in case
{
    return (int) xyz.x + ((int) xyz.y * (int) max.x) + ((int) xyz.z * (int) max.x * (int) max.y);
}

// gets
static vec3_t line_intersection_to_zplane(vec3_t a, vec3_t b, float z_distance)
{
    static const vec3_t normal = { 0.0f, 0.0f, 1.0f };
    vec3_t ab = HMM_SubtractVec3(b, a);

    float t = (z_distance - HMM_DotVec3(normal, a)) / HMM_DotVec3(normal, ab);

    vec3_t result = HMM_AddVec3(a, HMM_MultiplyVec3f(ab, t));

    return result;
}

typedef struct
{
    camera_t* camera;
    mat4_t inverse_proj;

    int first_cluster_index;
    int length;
} cluster_thread_input_t;

typedef struct
{
    int id;
    pthread_t thread;
} thread_wrapper_t;

// 10x10x10 clusters: 10 threads
// each thread basically handles each "screen" of clusters by each depth
static void* _thread_clustergen(void* ptr)
{
    // note: each thread will never contact the same memory cells.

    // for viewspace calcs.
    static const vec3_t position = { 0.0f };

    // get input (getting by value should be safer here but as long as we dont write we should be good)
    cluster_thread_input_t input = *((cluster_thread_input_t*) ptr); // void* to cluster_thread_input_t* cast

    for (int i = 0; i < input.length; i++) // loop through all clusters assigned to this thread.
                                           // this is basically what a single gpu thread would do
                                           // for this function in the original article
    {
        int cluster_index = input.first_cluster_index + i;
        vec3_t tile_coord = _index_to_xyz(cluster_index, CLUSTER_GRID_DIMENSIONS);

        // screenspace
        vec4_t max_point = HMM_Vec4((tile_coord.x + 1) * screen_tile_size, (tile_coord.y + 1) * screen_tile_size, -1.0f, 1.0f);
        vec4_t min_point = HMM_Vec4(tile_coord.x * screen_tile_size, tile_coord.y * screen_tile_size, -1.0f, 1.0f);

        // viewspace
        vec3_t vs_max_point = _screenspace_to_viewspace(input.camera, input.inverse_proj, max_point).xyz;
        vec3_t vs_min_point = _screenspace_to_viewspace(input.camera, input.inverse_proj, min_point).xyz;

        // cluster near/far in viewspace
        float cluster_near = -input.camera->near * powf(input.camera->far / input.camera->near, tile_coord.z / 10.0f);
        float cluster_far = -input.camera->near * powf(input.camera->far / input.camera->near, (tile_coord.z + 1.0f) / 10.0f);

        // calculate the aabb of this cluster grid cell (WE ARE USING VEC4 TO ADVANTAGE OF SIMD)
        vec4_t min_point_near = { .xyz = line_intersection_to_zplane(input.camera->transform.position, vs_min_point, cluster_near), .w = 0.0f };
        vec4_t min_point_far = { .xyz = line_intersection_to_zplane(input.camera->transform.position, vs_min_point, cluster_far), .w = 0.0f };
        vec4_t max_point_near = { .xyz = line_intersection_to_zplane(input.camera->transform.position, vs_max_point, cluster_near), .w = 0.0f };
        vec4_t max_point_far = { .xyz = line_intersection_to_zplane(input.camera->transform.position, vs_max_point, cluster_far), .w = 0.0f };

        vec4_t min_point_aabb = _fast_vec4_min(_fast_vec4_min(min_point_near, min_point_far), _fast_vec4_min(max_point_near, max_point_far));
        vec4_t max_point_aabb = _fast_vec4_max(_fast_vec4_max(min_point_near, min_point_far), _fast_vec4_max(max_point_near, max_point_far));

        cluster_grid[cluster_index] = (cluster_t) { min_point_aabb, max_point_aabb };
    }

    return NULL;
}


// this happens ONLY when the fov/znear/zfar changes.
void software_generate_cluster_grid(camera_t* camera)
{
    // in this case using glReadPixels() is okay because the prepass is already done - meaning that 
    // sync will NOT be an issue - HOORAY!
    // however:
    // we CANNOT take advantage of multiple threads with glReadPixels() i think it could work if the entire depth buffer was read to 
    // a buffer here in cpuland in a previously malloced buffer and then performs operations
    // simultaneously:
    // this can introduce a memory issue as a depth buffer for a 1920x1080 render target would take up
    // 1920px * 1080px * 1 float * 4 bytes/float = 8294400 bytes = 8.29 MB of ram usage for this portion of clustering alone

    // parallelized (to the best of my efforts)
    
    // take advantage of our cpu depth_buffer
    // glReadPixels(0, 0, CHOKS_WIDTH, CHOKS_HEIGHT, GL_DEPTH_COMPONENT, GL_FLOAT, (void*)depth_buffer);

    thread_wrapper_t threads[DEPTH_SLICE_COUNT];
    cluster_thread_input_t inputs[DEPTH_SLICE_COUNT];

    mat4_t invproj = _mat4_inverse(camera->matrices.projection);

    // pt. 1: initial cluster gen
    for (int i = 0; i < DEPTH_SLICE_COUNT; i++)
    {
        inputs[i].camera = camera;
        inputs[i].inverse_proj = invproj;
        inputs[i].length = (TOTAL_CLUSTER_COUNT / DEPTH_SLICE_COUNT);
        inputs[i].first_cluster_index = i * inputs[i].length;

        threads[i].id = pthread_create(&threads[i].thread, NULL, _thread_clustergen, (void*) &inputs[i]);
    }

    // wait for all threads to finish.
    for (int i = 0; i < DEPTH_SLICE_COUNT; i++)
    {
        pthread_join(threads[i].thread, NULL);
    }
}

void software_cull_clusters(); // TODO: implement active cluster detection

// this function won't be as "smart" as the one from the article, but it'll probably
// be quicker writing it this way on the CPU.
// goal: iterate through every cluster
typedef struct
{
    int first_cluster_index;
    int length;

    camera_t* camera;

    light_t* lights;
    int light_count;
} clusterpop_input_t;

// for point lights
static float sq_dist_vec3_aabb(vec3_t point, cluster_t* cluster, int cluster_id)
{
    float distance = 0.0f;

    cluster->max.z = cluster_id;

    for (int i = 0; i < 3; ++i)
    {
        float v = point.elements[i];
        if (v < cluster->min.elements[i])
        {
            distance += (cluster->min.elements[i] - v) * (cluster->min.elements[i] - v);
        }
        
        if (v > cluster->max.elements[i])
        {
            distance += (v - cluster->max.elements[i]) * (v - cluster->max.elements[i]);
        }
    }

    return distance;
}

static int test_sphere_aabb(camera_t* camera, light_t* light, cluster_t* cluster, int cluster_id)
{
    float radius = light->strength;

    vec3_t center = HMM_MultiplyMat4ByVec4(camera->matrices.view, (vec4_t) { light->position.xyz, 0.0f }).xyz;
    
    float squared_distance = sq_dist_vec3_aabb(center, cluster, cluster_id);

    return squared_distance <= (radius * radius);
}

pthread_mutex_t _lightarray_mutex;

static void* _thread_clusterpop(void* ptr)
{
    clusterpop_input_t input = *((clusterpop_input_t*) ptr);

    for (int i = 0; i < input.length; i++) // for each cluster
    {
        int cluster_index = input.first_cluster_index + i;

        int visible_light_count = 0;
        int visible_light_indices[MAX_LIGHTS_IN_CLUSTER];

        for (int j = 0; j < input.light_count; j++)
        {
            // check each light against the aabb of this cluster
            if (test_sphere_aabb(input.camera, &input.lights[j], &cluster_grid[cluster_index], cluster_index))
            {
                // if so add that to the list
                visible_light_indices[visible_light_count] = j;
                visible_light_count++; 
            }
        }

        pthread_mutex_lock(&_lightarray_mutex);
        int offset = global_light_index_count;
        global_light_index_count += visible_light_count;
        pthread_mutex_unlock(&_lightarray_mutex);

        for (int i = 0; i < visible_light_count; i++)
        {
            global_light_index_list[offset + i] = visible_light_indices[i];
        }

        cluster_lights[cluster_index].offset = offset;
        cluster_lights[cluster_index].length = visible_light_count;
    }

    return NULL;
}

void software_populate_cluster_grid(camera_t* camera, light_t* lights, int light_count) // per frame
{
    thread_wrapper_t threads[DEPTH_SLICE_COUNT];
    clusterpop_input_t inputs[DEPTH_SLICE_COUNT];

    pthread_mutex_init(&_lightarray_mutex, NULL);
    
    for (int i = 0; i < DEPTH_SLICE_COUNT; i++)
    {
        inputs[i].lights = lights;
        inputs[i].light_count = light_count;

        inputs[i].length = (TOTAL_CLUSTER_COUNT / DEPTH_SLICE_COUNT);
        inputs[i].first_cluster_index = i * inputs[i].length;

        threads[i].id = pthread_create(&threads[i].thread, NULL, _thread_clusterpop, NULL);
    }

    for (int i = 0; i < DEPTH_SLICE_COUNT; i++)
    {
        pthread_join(threads[i].thread, NULL);
    }

    pthread_mutex_destroy(&_lightarray_mutex);
}