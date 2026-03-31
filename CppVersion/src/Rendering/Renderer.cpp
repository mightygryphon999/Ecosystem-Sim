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
constexpr int kScreenHeight{ 480 };

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
                gScreenRenderer = SDL_CreateRenderer(gWindow,"main");
                if (gScreenRenderer == nullptr) {
                    SDL_Log("Couldnt get the renderer or create it");
                    success = false;
                }
            }
        }

        return success;
    }

    bool loadMedia();

    void present() {
        SDL_GetRenderDrawColor(gScreenRenderer,reinterpret_cast<Uint8 *>(255),reinterpret_cast<Uint8 *>(255),reinterpret_cast<Uint8 *>(255),reinterpret_cast<Uint8 *>(255));
        SDL_RenderClear(gScreenRenderer);
        SDL_RenderPresent(gScreenRenderer);
    }

    void present_chunk_grid() {
        for (int x = 0; x < 10; x++) {
            for (int y = 0; y < 10; y++) {
                SDL_FRect rect {World::chunks_map_main[x][y].x, World::chunks_map_main[x][y].y, World::chunks_map_main[x][y].Cw, World::chunks_map_main[x][y].Cw};
                SDL_SetRenderDrawColor(gScreenRenderer, 0, 0, 0, 255);
                SDL_RenderRect(gScreenRenderer, &rect);
            }
        }
    }

    void close() {
        SDL_DestroyWindow(gWindow);
        SDL_DestroyRenderer(gScreenRenderer);
        gWindow = nullptr;
        gScreenRenderer = nullptr;

        SDL_Quit();
    }
}