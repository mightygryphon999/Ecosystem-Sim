//
// Created by Griffin Roberts on 4/3/26.
//

#ifndef C__VERSION_UIEDITORSETTINGSRENDERER_H
#define C__VERSION_UIEDITORSETTINGSRENDERER_H

#include "UIEditorSettingsObjects/UIViewTargetLineCheckbox.h"
#include <SDL3/SDL.h>

namespace UIEditorSettingsRenderer {
        inline UIViewTargetLineCheckbox::UIViewTargetLineCheckbox target_line_checkbox = {};

        int input(const SDL_Event &e);
        int render(SDL_Renderer* renderer);
} // UIEditorSettingsRenderer

#endif //C__VERSION_UIEDITORSETTINGSRENDERER_H
