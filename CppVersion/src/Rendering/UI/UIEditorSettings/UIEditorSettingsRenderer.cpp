//
// Created by Griffin Roberts on 4/3/26.
//

#include "UIEditorSettingsRenderer.h"
#include <SDL3/SDL.h>

namespace UIEditorSettingsRenderer {
    int input(const SDL_Event &e) {
        target_line_checkbox.handle_event(e);
        return 0;
    }

    int render(SDL_Renderer* renderer) {
        target_line_checkbox.render(renderer);
        return 0;
    }
} // UIEditorSettingsRenderer