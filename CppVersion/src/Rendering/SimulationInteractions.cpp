//
// Created by Griffin Roberts on 4/1/26.
//

#include "SimulationInteractions.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <cmath>
#include "../config.h"
#include "../Simulation/World.h"

namespace SimulationInteractions {
    void mouse_interaction(const SDL_Event* e, float mouse_x, float mouse_y) {
        Uint32 mouseState = SDL_GetMouseState(NULL, NULL);
        if (mouseState & SDL_BUTTON_LMASK) {
            // Adds a new test point
            int flooredX = std::floor(mouse_x/WORLD_CHUNK_SIZE);
            int flooredY = std::floor(mouse_y/WORLD_CHUNK_SIZE);

            World::chunks_map_main[flooredX][flooredY].test_points.push_back({mouse_x,mouse_y,true});
        }
        if (mouseState & SDL_BUTTON_RMASK) {
            // Adds a new test point
            int flooredX = std::floor(mouse_x/WORLD_CHUNK_SIZE);
            int flooredY = std::floor(mouse_y/WORLD_CHUNK_SIZE);

            World::chunks_map_main[flooredX][flooredY].test_points.push_back({mouse_x,mouse_y,false});
        }
    }
} // SimulationInteractions