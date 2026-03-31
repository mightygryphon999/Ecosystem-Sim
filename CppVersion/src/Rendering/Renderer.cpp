//
// Created by Griffin Roberts on 3/30/26.
//

#include "Renderer.h"
#include "../Simulation/World.h"
#include "../Simulation/Chunk.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>

constexpr int kScreenWidth{ 640 };
constexpr int kScreenHeight{ 640 };

namespace Renderer {
    // Global Variables
    SDL_Window* gWindow{ nullptr };
    SDL_Renderer* gScreenRenderer{ nullptr };

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
                gScreenRenderer = SDL_CreateRenderer(gWindow,nullptr);
                if (gScreenRenderer == nullptr) {
                    SDL_Log("Couldnt get the renderer or create it");
                    success = false;
                }
            }
        }

        return success;
    }

    bool loadMedia();

    void present_chunk_grid() {
        constexpr float kCellSize = 64.0f;

        for (int x = 0; x < 10; x++) {
            for (int y = 0; y < 10; y++) {
                SDL_FRect rect {x * kCellSize, y * kCellSize, kCellSize, kCellSize};
                SDL_SetRenderDrawColor(gScreenRenderer, 255, 255, 255, 255);
                SDL_RenderRect(gScreenRenderer, &rect);
            }
        }
    }

    void present(float mouseX, float mouseY) {
        SDL_SetRenderDrawColor(gScreenRenderer,0,0,0,255);
        SDL_RenderClear(gScreenRenderer);
        present_chunk_grid();
        SDL_RenderPresent(gScreenRenderer);
    }

    void close() {
        SDL_DestroyWindow(gWindow);
        SDL_DestroyRenderer(gScreenRenderer);
        gWindow = nullptr;
        gScreenRenderer = nullptr;

        SDL_Quit();
    }
}