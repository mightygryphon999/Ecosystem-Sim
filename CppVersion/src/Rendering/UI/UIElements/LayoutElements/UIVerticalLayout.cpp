//
// Created by Griffin Roberts on 4/4/26.
//

#include "UIVerticalLayout.h"
#include "../UIVerticalElement.h"


void UIVerticalLayout::layout() {
    int currentY = y;

    for (int i = 0; i < children.size(); i++) {
        children[i]->y = currentY;
        currentY += padding + spacing;
    }
}

void UIVerticalLayout::render(SDL_Renderer *renderer) {
    for (auto* child : children) {
        child->render(renderer);
    }
}
