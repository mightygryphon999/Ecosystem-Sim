//
// Created by Griffin Roberts on 4/4/26.
//

#ifndef C__VERSION_UIVERTICALLAYOUT_H
#define C__VERSION_UIVERTICALLAYOUT_H

#include <vector>
#include "../UIVerticalElement.h"
#include "../UIElement.h"
#include <SDL3/SDL.h>

struct UIVerticalLayout : UIElement {
    std::vector<UIVerticalElement::UIVerticalElement*> children;
    int spacing = 5;
    int padding = 0;

    void layout();
    void render(SDL_Renderer* renderer) override;
};

#endif //C__VERSION_UIVERTICALLAYOUT_H
