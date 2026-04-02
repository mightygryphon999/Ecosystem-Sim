//
// Created by Griffin Roberts on 3/30/26.
//

#ifndef C__VERSION_RENDERER_H
#define C__VERSION_RENDERER_H

#include <SDL3/SDL.h>

namespace Renderer {
    bool init();
    void handle_input(const SDL_Event* e);
    void present(float mouseX, float mouseY, float mouseScrollY, float fps);
    void close();
} // Renderer

#endif //C__VERSION_RENDERER_H
