//
// Created by Griffin Roberts on 4/3/26.
//

#ifndef C__VERSION_UICHECKBOX_H
#define C__VERSION_UICHECKBOX_H

#include <SDL3/SDL.h>

namespace UICheckbox {
    struct UICheckbox {
        SDL_FRect rect;
        bool hovered{ false };
        bool pressed{ false };
        bool state{ true };

        virtual void handle_clicked() {}
        virtual void handle_event(const SDL_Event &e) {}
        virtual void render(SDL_Renderer* renderer) {}
    };
}

#endif //C__VERSION_UICHECKBOX_H
