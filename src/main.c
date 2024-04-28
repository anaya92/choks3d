// primitives only bro
#include "turan_choks.h"
#include "upper_graphics.h"
#include "lolita.h"

#include "rskybox.h"

#include <SDL2/SDL.h>
#include <stdio.h>

#include <unistd.h>
#define CWD "./pmc"

float __planevertices[] = {
    -0.5f, 0.0f, -0.5f, 0.0f, 1.0f,
    -0.5f, 0.0f, 0.5f, 0.0f, 0.0f,
    0.5f, 0.0f, -0.5f, 1.0f, 1.0f,
    0.5f, 0.0f, 0.5f, 1.0f, 0.0f,
};

float __triangletest[] = {
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
     0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
     0.0f,  0.5f, 0.0f, 0.5f, 1.0f,
};

int main(int argc, char* argv[])
{
    chdir(CWD);

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window* window = SDL_CreateWindow(
        "[choks]",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        CHOKS_WIDTH,
        CHOKS_HEIGHT,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
    );
    SDL_GLContext sdl_gl_context = SDL_GL_CreateContext(window);

    if (gladLoadGL((GLADloadfunc) SDL_GL_GetProcAddress) == 0)
    {
        printf("glad failed to load!\n");
    }
    glClearColor(0.0f, 0.512f, 0.512f, 1.0f);

    setup_choks();
    setup_lolkim();

    unsigned int indices[] = {
        0, 1, 2, 1, 2, 3
    };
    primitive_t plane = primitive_load_with_indices(__planevertices, 4, indices, 6, GL_TRIANGLES);

    program_t program = program_load_from_files("gfx/src/basic.v.glsl", "gfx/src/basic.f.glsl");    
    program_t point_program = program_load_from_files("gfx/src/basic.v.glsl","gfx/src/points.f.glsl");

    camera_t camera = { 0 };
    camera.transform.position = (vec3_t) { 0.0f, 0.0f, -10.0f };
    camera.transform.rotate = (vec3_t) { 0.0f, 90.0f, 0.0f };
    camera.fov = 120.0f;
    camera.aspect = 4.0/3.0f;
    camera.near = 0.1f;
    camera.far = 1000.0f;

    camera_update_projection(&camera);

    rskybox_setup();
    texture_t cubemap = texture_load_cubemap_from_file("media/skybox/space.webp");

    texture_t scrolling = texture_load_2d_from_file("media/misc/noise.webp");
    program_t water_program = program_load_from_files("gfx/src/water.v.glsl", "gfx/src/water.f.glsl");

    // gl configuration
    glPointSize(50.0f);
    glViewport(0, 0, CHOKS_WIDTH, CHOKS_HEIGHT);
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

    float delta = 0.0f;

    float object_rotation = 0.0f;
    float height = 0.0f;

    uint64_t now = SDL_GetPerformanceCounter();
    uint64_t last = 0;

    transform_t trans_plane = { 0 };
    trans_plane.scale = (vec3_t) { 10.0f, 0.0f, 10.0f };

    int time_loc = glGetUniformLocation(water_program.id, "time");

    // printf("frametime (secs): %10f", 0.0f);

    float avg_delta = 1.0f;

    int running = 1;
    while (running)
    {
        last = now;
        now = SDL_GetPerformanceCounter();

        delta = (float)((float)(now - last) / (float)SDL_GetPerformanceFrequency() );
        avg_delta = (avg_delta + delta) / 2.0f;

        // printf("\b\b\b\b\b\b\b\b\b\b%10f", delta * 1000.0f);

        // test the cluster grid generation speed
        // perftest(&camera);
        // HOLY SHIT SIMD N MULTITHREAD IS SO FACKING FAST frametime avg = 1.25 ms
        // without running this func every frame the time is 0.81 ms/frame
        // worst-case predicted performance hit of 0.39 ms

        SDL_Event ev;
        while (SDL_PollEvent(&ev))
        {
            switch(ev.type)
            {
                case SDL_QUIT: running = 0; break;
                case SDL_KEYDOWN:
                    switch (ev.key.keysym.scancode)
                    {
                        case SDL_SCANCODE_F10:
                            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
                            break;
                        case SDL_SCANCODE_ESCAPE:
                            running = 0;
                            break;
                        case SDL_SCANCODE_Z:
                            camera.fov = 60.0f;
                            camera_update_projection(&camera);
                            break;
                        default: break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (ev.key.keysym.scancode)
                    {
                        case SDL_SCANCODE_Z:
                            camera.fov = 120.0f;
                            camera_update_projection(&camera);
                            break;
                        default: break;
                    }
                    break;
            }
        }

        const unsigned char* kb = SDL_GetKeyboardState(NULL);
        
        if (kb[SDL_SCANCODE_SPACE]) camera.transform.position.y += 10.0f * delta;
        if (kb[SDL_SCANCODE_LSHIFT]) camera.transform.position.y -= 10.0f * delta;

        if (kb[SDL_SCANCODE_UP]) camera.transform.rotate.x += 50.0f * delta;
        if (kb[SDL_SCANCODE_DOWN]) camera.transform.rotate.x -= 50.0f * delta;
        if (kb[SDL_SCANCODE_LEFT]) camera.transform.rotate.y -= 50.0f * delta;
        if (kb[SDL_SCANCODE_RIGHT]) camera.transform.rotate.y += 50.0f * delta;

        object_rotation += 1000.0f * delta;

        mat4_t obj_trans = HMM_Mat4d(1.0f);
        // obj_trans = HMM_MultiplyMat4(obj_trans, HMM_Scale((hmm_vec3) {3.0f}));
        //obj_trans = HMM_MultiplyMat4(obj_trans, HMM_Rotate(HMM_ToRadians(45.0f), HMM_Vec3(1.0f, 0.0f, 0.0f)));
        obj_trans = HMM_MultiplyMat4(obj_trans, HMM_Rotate(HMM_ToRadians(object_rotation), HMM_Vec3(0.0f, 1.0f, 0.0f)));
        obj_trans = HMM_MultiplyMat4(obj_trans, HMM_Rotate(HMM_ToRadians(object_rotation), HMM_Vec3(1.0f, 0.0f, 0.0f)));
        set_model_matrix(obj_trans);

        camera_update_view(&camera);
        set_view_and_projection_matrices(camera.matrices.view, camera.matrices.projection); // FIXME: should just add another function
                                                                                            // so i can set each matrix separately
        
        // FIXME: this logic is flawed b/c the movement data doesnt happen until the next frame. however, this stuff in a real game would ideally
        // happen AFTER all the parent character's movement had been calculate so i dont really have to worry too much about it
        if (kb[SDL_SCANCODE_W]) camera.transform.position = HMM_AddVec3(camera.transform.position, HMM_MultiplyVec3f(camera.front, 5.0f * delta));
        if (kb[SDL_SCANCODE_S]) camera.transform.position = HMM_SubtractVec3(camera.transform.position, HMM_MultiplyVec3f(camera.front, 5.0f * delta));
        // if (kb[SDL_SCANCODE_A]) camera.position = HMM_AddVec3(camera.position, HMM_MultiplyVec3f(temp_front, 5.0f * delta));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program.id);
        plane.draw_mode = GL_TRIANGLES;
        primitive_draw(&plane);

        set_model_matrix(transform_to_matrix(&trans_plane));
        glUseProgram(point_program.id);
        plane.draw_mode = GL_POINTS;
        primitive_draw(&plane);

        rskybox_render(cubemap);
        
        // transparent objects have to be rendered last. UGH
        plane.draw_mode = GL_TRIANGLES;
        glUseProgram(water_program.id);
        glUniform1f(time_loc, (float) ((float)SDL_GetTicks() / 1000.0f));
        glBindTexture(GL_TEXTURE_2D, scrolling.id);
        primitive_draw(&plane);

        SDL_GL_SwapWindow(window);
    }

    printf("\n\nshutting down. avg slimetime %f\n", avg_delta * 1000);

    program_free(water_program);
    texture_free(scrolling);

    rskybox_cleanup();
    texture_free(cubemap);

    program_free(point_program);
    program_free(program);
    primitive_free(&plane);

    cleanup_choks();

    SDL_GL_DeleteContext(sdl_gl_context);
    SDL_DestroyWindow(window);

    SDL_Quit();
}