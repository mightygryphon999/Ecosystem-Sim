//
// Created by Griffin Roberts on 3/30/26.
//

#include "Renderer.h"
#include "../Simulation/World.h"
#include "../Simulation/Chunk.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include "Visualizers/ChunkNeighborsVisualizer.h"

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

    void handle_input(const SDL_Event* e) {
        ChunkNeighborsVisualizer::handle_chunk_input(e);
    }

    void present(float mouseX, float mouseY, float mouseScrollY) {
        SDL_SetRenderDrawColor(gScreenRenderer,0,0,0,255);
        SDL_RenderClear(gScreenRenderer);
        ChunkNeighborsVisualizer::present_chunk_neighbors_visualizer(gScreenRenderer, mouseX, mouseY, mouseScrollY);
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