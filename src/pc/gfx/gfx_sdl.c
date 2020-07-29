#if 0
#include <stdlib.h>
#include <stdbool.h>

#if FOR_WINDOWS
#include <GL/glew.h>
#include "SDL.h"
#define GL_GLEXT_PROTOTYPES 1
#include "SDL_opengl.h"
#else
#include <SDL2/SDL.h>
#define GL_GLEXT_PROTOTYPES 1
#include <SDL2/SDL_opengles2.h>
#endif

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define HALF_SCREEN_WIDTH (SCREEN_WIDTH / 2)
#define HALF_SCREEN_HEIGHT (SCREEN_HEIGHT / 2)

#define DESIRED_SCREEN_WIDTH (SCREEN_WIDTH * 4)
#define DESIRED_SCREEN_HEIGHT (SCREEN_HEIGHT * 4)

static SDL_Window *wnd;

static void gfx_sdl_init(void) {
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    
    wnd = SDL_CreateWindow("Super Mario 64 PC-Port", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            DESIRED_SCREEN_WIDTH, DESIRED_SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    
    SDL_GL_CreateContext(wnd);
    SDL_GL_SetSwapInterval(0); // TODO 0, 1 or 2 or remove this line?
}

static void gfx_sdl_handle_events(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                exit(0);
        }
    }
}

static void swap_buffers() {
    uint8_t *driverdata = *(uint8_t **)((uint8_t *)wnd + 192);
    uintptr_t xwindow = *(uintptr_t *)(driverdata + sizeof(void *));
    uint8_t *videodata = *(uint8_t **)(driverdata + 224);
    uint8_t *display = *(uint8_t **)videodata;
    
    bool (*glXGetSyncValuesOML)(uint8_t *, uintptr_t, int64_t *, int64_t *, int64_t *) = SDL_GL_GetProcAddress("glXGetSyncValuesOML");
    
    bool (*glXGetMscRateOML)(uint8_t *, uintptr_t, int32_t *, int32_t *) = SDL_GL_GetProcAddress("glXGetMscRateOML");
    
    int64_t (*glXSwapBuffersMscOML)(uint8_t *, uintptr_t, int64_t, int64_t, int64_t) = SDL_GL_GetProcAddress("glXSwapBuffersMscOML");
    
    bool (*glXWaitForSbcOML)(uint8_t *, uintptr_t, int64_t, int64_t *, int64_t *, int64_t *) = SDL_GL_GetProcAddress("glXWaitForSbcOML");
    
    static int64_t last_msc;
    
    bool res;
    int64_t ust, msc, sbc;
    res = glXGetSyncValuesOML(display, xwindow, &ust, &msc, &sbc);
    printf("%d %ld %ld %ld\n", res, ust, msc, sbc);
    
    int32_t numerator, denominator;
    res = glXGetMscRateOML(display, xwindow, &numerator, &denominator);
    printf("%d %d %d\n", res, numerator, denominator);
    int32_t fps = numerator / denominator;
    
    int64_t next_msc = last_msc + (fps / 30);
    
    int64_t sbc_at_swap = glXSwapBuffersMscOML(display, xwindow, next_msc, 0, 0);
    
    res = glXWaitForSbcOML(display, xwindow, 0, &ust, &msc, &sbc);
    if (msc != next_msc) {
        printf("FRAME too late!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! %ld %ld\n", sbc, msc - next_msc);
    }
    last_msc = msc;
}

static void gfx_sdl_swap_buffers(void) {
    //SDL_GL_SwapWindow(wnd);
    swap_buffers();
}

void gfx_window_system_init(void) {
    gfx_sdl_init();
}

void gfx_window_system_handle_events(void) {
    gfx_sdl_handle_events();
}

void gfx_window_system_swap_buffers(void) {
    gfx_sdl_swap_buffers();
}
#endif
