//
// Created by Griffin Roberts on 4/4/26.
//

#ifndef C__VERSION_UIELEMENT_H
#define C__VERSION_UIELEMENT_H
#include <SDL3/SDL_render.h>

struct UIElement {
    float x, y;

    virtual void render(SDL_Renderer* renderer){}
};

#endif //C__VERSION_UIELEMENT_H
