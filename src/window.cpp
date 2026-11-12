#include "window.h"

#include <iostream>

bool window::initialize(const char* name, int width, int height) {
    // Init SDL window
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL init failed: " << SDL_GetError() << std::endl;
        return false;
    }

    // Set SDL GL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create SDL window
    sdl_window = SDL_CreateWindow(
        name,
        width,
        height,
        SDL_WINDOW_OPENGL
    );

    gl_context = SDL_GL_CreateContext(sdl_window);
    
    // Set GLAD
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    return true;
}

void window::poll_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT)
            should_close = true;
        if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE)
            should_close = true;
    }
}

void window::swap_buffers() {
    SDL_GL_SwapWindow(sdl_window);
}

window::~window() {
    // Cleanup
    SDL_GL_DestroyContext(gl_context);
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();
}
