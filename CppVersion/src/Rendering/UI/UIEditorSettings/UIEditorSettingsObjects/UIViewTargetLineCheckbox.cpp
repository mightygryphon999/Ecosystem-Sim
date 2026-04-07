//
// Created by Griffin Roberts on 4/3/26.
//

#include "UIViewTargetLineCheckbox.h"

#include <iostream>
#include <ostream>
#include <SDL3/SDL.h>
#include "../../../Visualizers/ChunkNeighborsVisualizer.h"
#include "../../../Renderer.h"

namespace UIViewTargetLineCheckbox {
    void UIViewTargetLineCheckbox::handle_clicked() {
        state = !state;
        ChunkNeighborsVisualizer::display_test_targets = state;
    }

    void UIViewTargetLineCheckbox::handle_event(const SDL_Event &e) {
        if (e.type == SDL_EVENT_MOUSE_MOTION) {
            const SDL_FPoint point = {static_cast<float>(e.motion.x), static_cast<float>(e.motion.y)};
            hovered = SDL_PointInRectFloat(&point, &rect);
        }

        if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && hovered) {
            pressed = true;
        }

        if (e.type == SDL_EVENT_MOUSE_BUTTON_UP) {
            if (pressed && hovered) {
                handle_clicked();
            }
            pressed = false;
        }
    }

    void UIViewTargetLineCheckbox::render(SDL_Renderer *renderer) {
        rect = SDL_FRect{x, y, 40,40};

        if (pressed) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 1.0f);
        }
        else if (hovered) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 255, 1.0f);
        } else if (state) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 1.0f);
        }
        else {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 1.0f);
        }

        SDL_RenderFillRect(renderer, &rect);
    }
}
