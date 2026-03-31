//
// Created by Griffin Roberts on 3/30/26.
//

#ifndef C__VERSION_RENDERER_H
#define C__VERSION_RENDERER_H

#include <SDL3/SDL.h>

namespace Renderer {
    bool init();
    void present(float mouseX, float mouseY, float mouseScrollY);
    void close();
} // Renderer

#endif //C__VERSION_RENDERER_H
