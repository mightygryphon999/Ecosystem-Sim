//
// Created by Griffin Roberts on 4/3/26.
//

#include "UIEditorSettingsRenderer.h"
#include <SDL3/SDL.h>
#include "../UIElements/LayoutElements/UIVerticalLayout.h"

namespace UIEditorSettingsRenderer {
    UIVerticalLayout settings_layout;

    int init() {

    }

    int input(const SDL_Event &e) {
        target_line_checkbox.handle_event(e);
        return 0;
    }

    int render(SDL_Renderer* renderer) {
        target_line_checkbox.x = 650;
        target_line_checkbox.y = 100;
        target_line_checkbox.render(renderer);
        return 0;
    }
} // UIEditorSettingsRenderer