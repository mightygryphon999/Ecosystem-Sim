//
// Created by Griffin Roberts on 4/1/26.
//

#ifndef C__VERSION_FONT_H
#define C__VERSION_FONT_H

#include <SDL3/SDL.h>

namespace font {
    SDL_Texture* load_font(SDL_Renderer *renderer);
    void draw_char(SDL_Renderer *renderer, unsigned char character, float cx, float cy, int scale);
    void draw_text(SDL_Renderer *renderer, const char* text, int x, int y, int scale);
} // font

#endif //C__VERSION_FONT_H
