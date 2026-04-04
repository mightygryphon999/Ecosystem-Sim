//
// Created by Griffin Roberts on 4/4/26.
//

#ifndef C__VERSION_UIVERTICALELEMENT_H
#define C__VERSION_UIVERTICALELEMENT_H

#include "SDL3/SDL.h"

namespace UIVerticalElement {
    struct UIVerticalElement {
        float x, y;

        virtual void render(SDL_Renderer* renderer){}
    };
}

#endif //C__VERSION_UIVERTICALELEMENT_H
