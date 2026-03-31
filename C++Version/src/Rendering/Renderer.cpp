//
// Created by Griffin Roberts on 3/30/26.
//

#include "Renderer.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

constexpr int kScreenWidth{ 640 };
constexpr int kScreenHeight{ 480 };

namespace Renderer {
    // Global Variables
    SDL_Window* gWindow{ nullptr };
    SDL_Surface* gScreenSurface{ nullptr };

    bool init()
    {
        bool success{ true };

        if (SDL_Init(SDL_INIT_VIDEO) == false) {
            SDL_Log("SDL could not initialize! SDL Error: %s", SDL_GetError());
            success = false;
        }
        else {
            if (gWindow = SDL_CreateWindow("Ecosystem Simulator", kScreenWidth, kScreenHeight, 0); gWindow == nullptr) {
                SDL_Log("Window could not be created! SDL Error: %s", SDL_GetError());
            }
            else {
                gScreenSurface = SDL_GetWindowSurface(gWindow);
            }
        }

        return success;
    }

    bool loadMedia();

    void present() {
        SDL_FillSurfaceRect( gScreenSurface, nullptr, SDL_MapSurfaceRGB( gScreenSurface, 0xFF, 0xFF, 0xFF ) );
        SDL_UpdateWindowSurface( gWindow );
    }

    void close() {
        SDL_DestroyWindow(gWindow);
        gWindow = nullptr;
        gScreenSurface = nullptr;

        SDL_Quit();
    }
}