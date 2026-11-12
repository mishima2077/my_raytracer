#pragma once

#include "glad/glad.h"
#include <SDL3/SDL.h>

class window {
public:
    bool initialize(const char* name, int width, int height);

    // Sets should_close = true if window is closed or esc key is pressed
    void poll_events();

    void swap_buffers();

    ~window();

    SDL_Window* sdl_window;
    SDL_GLContext gl_context;
    bool should_close = false;
};
