//
// Created by Griffin Roberts on 3/31/26.
//

#include "ChunkNeighborsVisualizer.h"
#include "../../Simulation//World.h"
#include "../../Simulation/Chunk.h"
#include "../../Simulation/MathUtils.h"
#include "../../Simulation/TestClasses/TestPoint.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <cmath>
#include <iostream>
#include <SDL3/SDL_render.h>
#include <vector>

namespace ChunkNeighborsVisualizer {
    int range = 130;
    int mode = 2;

    void present_chunk_grid(SDL_Renderer* &renderer);

    void present_chunk_neighbors_filled(SDL_Renderer* &renderer, float mouse_x, float mouse_y, float mouseScrollY);

    void present_closest_connections_mouse(SDL_Renderer* &renderer, float mouse_x, float mouse_y, float mouseScrollY);

    void present_chunk_neighbors_visualizer(SDL_Renderer* &renderer, float mouse_x, float mouse_y, float mouseScrollY) {
        if (mode == 0) {
            present_chunk_neighbors_filled(renderer, mouse_x, mouse_y, mouseScrollY);
            present_chunk_grid(renderer);
        }
        else if (mode == 1) {
            present_closest_connections_mouse(renderer,mouse_x,mouse_y,mouseScrollY);
            present_chunk_grid(renderer);
        }
        else if (mode == 2) {
            present_closest_connections_mouse(renderer,mouse_x,mouse_y,mouseScrollY);
            present_chunk_neighbors_filled(renderer, mouse_x, mouse_y, mouseScrollY);
            present_chunk_grid(renderer);
        }
    }

    void present_closest_connections_mouse(SDL_Renderer* &renderer, float mouse_x, float mouse_y, float mouseScrollY) { // finds the closest connections to agents within the range of the mouse cursor
        range += mouseScrollY * 10.0f;
        range = std::round(range / 10.0f) * 10.0f;

        int flooredX = std::floor(mouse_x/64);
        int flooredY = std::floor(mouse_y/64);

        bool found_chunk{ false };
        for (int x = 0; x < 10; x++) {
            for (int y = 0; y < 10; y++) {
                if (flooredX == x && flooredY == y) {
                    found_chunk = true;
                    TestPoint::test_point point = MathUtils::find_nearest_connection_test_points(World::chunks_map_main[x][y], mouse_x, mouse_y, range);
                    SDL_SetRenderDrawColor(renderer, 255,255,255,1.0f);
                    SDL_RenderLine(renderer, mouse_x, mouse_y, point.x, point.y);
                    std::cout << "pos: " << mouse_x << ", " << mouse_y << " | point: "
                              << point.x << ", " << point.y
                              << " | dx=" << (mouse_x - point.x)
                              << " dy=" << (mouse_y - point.y) << "\n";
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
        //present_chunk_grid(renderer);
    }

    void present_chunk_neighbors_filled(SDL_Renderer* &renderer, float mouse_x, float mouse_y, float mouseScrollY) {
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
        //present_chunk_grid(renderer);
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