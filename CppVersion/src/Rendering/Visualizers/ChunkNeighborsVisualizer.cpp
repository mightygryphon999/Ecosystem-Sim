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

namespace ChunkNeighborsVisualizer {
    float range = 128;

    void present_chunk_neighbors_visualizer(SDL_Renderer &renderer, float mouse_x, float mouse_y) {
        for (int x = 0; x < 10; x++) {
            for (int y = 0; y < 10; y++) {
                int flooredX = std::floor(mouse_x/64);
                int flooredY = std::floor(mouse_y/64);

                if (flooredX == x && flooredY == y) {
                    
                }
            }
        }
    }
} // ChunkNeighborsVisualizer