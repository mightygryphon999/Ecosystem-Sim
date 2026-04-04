//
// Created by Griffin Roberts on 4/3/26.
//

#ifndef C__VERSION_UIVIEWTARGETLINECHECKBOX_H
#define C__VERSION_UIVIEWTARGETLINECHECKBOX_H

#include "../../UIElements/UICheckbox.h"
#include "../../../Renderer.h"
#include "../../UIElements/UIVerticalElement.h"

namespace UIViewTargetLineCheckbox {
    struct UIViewTargetLineCheckbox : UICheckbox::UICheckbox, UIVerticalElement::UIVerticalElement {
        SDL_FRect rect = SDL_FRect{x, y, 10,10};

        void handle_clicked() override;
        void handle_event(const SDL_Event &e) override;
        void render(SDL_Renderer *renderer) override;
    };

} // UIViewTargetLineCheckbox

#endif //C__VERSION_UIVIEWTARGETLINECHECKBOX_H
