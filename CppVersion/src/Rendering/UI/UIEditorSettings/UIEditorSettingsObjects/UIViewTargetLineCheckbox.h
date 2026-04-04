//
// Created by Griffin Roberts on 4/3/26.
//

#ifndef C__VERSION_UIVIEWTARGETLINECHECKBOX_H
#define C__VERSION_UIVIEWTARGETLINECHECKBOX_H

#include "../../UIElements/UICheckbox.h"
#include "../../../Renderer.h"

namespace UIViewTargetLineCheckbox {
    inline SDL_FRect rect = SDL_FRect{Renderer::kScreenWidth / 2 - 5, Renderer::kScreenHeight / 2 - 5, 10,10};

    struct UIViewTargetLineCheckbox : UICheckbox::UICheckbox {
        void handle_clicked() override;
        void handle_event(const SDL_Event &e) override;
        void render(SDL_Renderer *renderer) override;
    };

} // UIViewTargetLineCheckbox

#endif //C__VERSION_UIVIEWTARGETLINECHECKBOX_H
