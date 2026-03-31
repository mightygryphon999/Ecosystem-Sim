//
// Created by Griffin Roberts on 3/31/26.
//

#include "ChunkNeighborsVisualizer.h"
#include "../../Simulation//World.h"
#include "../../Simulation/Chunk.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <cmath>
#include <SDL3/SDL_render.h>
#include <vector>

namespace ChunkNeighborsVisualizer {
    int range = 130;

    void present_chunk_grid(SDL_Renderer* &renderer);

    void present_chunk_neighbors_visualizer(SDL_Renderer* &renderer, float mouse_x, float mouse_y, float mouseScrollY) {
        bool found_chunk{ false };
        int flooredX = std::floor(mouse_x/64);
        int flooredY = std::floor(mouse_y/64);
        range += mouseScrollY * 10.0f;
        range = std::round(range / 10.0f) * 10.0f;
        for (int x = 0; x < 10; x++) {
            for (int y = 0; y < 10; y++) {
                World::chunks_map_main[x][y].visuals.filled = false;
            }
        }
        for (int x = 0; x < 10; x++) {
            for (int y = 0; y < 10; y++) {
                if (flooredX == x && flooredY == y) {
                    found_chunk = true;
                    for (ChunkData::Chunk* &chunk : World::chunks_map_main[x][y].cached_chunks[range].chunks) {
                        chunk->visuals.filled = true;
                    }
                    break;
                }

                if (found_chunk) {
                    break;
                }
            }
            if (found_chunk) {
                break;
            }
        }
        present_chunk_grid(renderer);
    }

    void present_chunk_grid(SDL_Renderer* &renderer) {
        constexpr float kCellSize = 64.0f;

        for (int x = 0; x < 10; x++) {
            for (int y = 0; y < 10; y++) {
                SDL_FRect rect {x * kCellSize, y * kCellSize, kCellSize, kCellSize};
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                if (World::chunks_map_main[x][y].visuals.filled) {
                    SDL_RenderFillRect(renderer, &rect);
                }
                else {
                    SDL_RenderRect(renderer, &rect);
                }
            }
        }
    }
} // ChunkNeighborsVisualizer