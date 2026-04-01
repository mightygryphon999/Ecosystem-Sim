//
// Created by Griffin Roberts on 4/1/26.
//

#include "font.h"
#include <SDL3/SDL.h>
#include "font_bitmap.h"

namespace font {
    static SDL_Texture* font_tex;

    SDL_Texture* load_font(SDL_Renderer *renderer) {
        SDL_Surface *surface = SDL_CreateSurface(FONT_WIDTH, FONT_HEIGHT,
            SDL_GetPixelFormatForMasks(32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000));

        Uint32 white = SDL_MapSurfaceRGB(surface, 255, 255, 255);
        Uint32 black = SDL_MapSurfaceRGBA(surface, 0, 0, 0, 0);

        for (int y = 0; y < FONT_HEIGHT; ++y) {
            for (int x = 0; x < FONT_WIDTH; ++x) {
                Uint32 color = font_bitmap[y][x] ? white : black;
                ((Uint32 *)surface->pixels)[y * FONT_WIDTH + x] = color;
            }
        }

        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
        SDL_DestroySurface(surface);

        return texture;
    }
    void draw_char(SDL_Renderer *renderer, unsigned char character, float cx, float cy, int scale) {
        if (font_tex == nullptr)
            font_tex = load_font(renderer);
        SDL_Point cell = {
            (character-32) % (FONT_WIDTH/GLYPH_WIDTH),
            (character-32) / (FONT_WIDTH/GLYPH_WIDTH) };
        SDL_FRect srcrect = {
            static_cast<float>(cell.x * GLYPH_WIDTH),
            static_cast<float>(cell.y * GLYPH_HEIGHT),
            GLYPH_WIDTH,
            GLYPH_HEIGHT };
        SDL_FRect dstrect = {cx,cy,static_cast<float>(GLYPH_WIDTH * scale),static_cast<float>(GLYPH_HEIGHT * scale) };

        SDL_RenderTexture(renderer, font_tex, &srcrect, &dstrect);
    }
    void draw_text(SDL_Renderer *renderer, const char* text, int x, int y, int scale) {
        for (size_t i = 0; text[i] != '\0'; i++) {
            draw_char(renderer, text[i], x + i * GLYPH_WIDTH * scale, y, scale);
        }
    }
} // font