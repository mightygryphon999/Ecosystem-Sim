//
// Created by Griffin Roberts on 3/31/26.
//

#include "ChunkNeighborsVisualizer.h"
#include "../../Simulation//World.h"
#include "../../config.h"
#include "../../Simulation/Chunk.h"
#include "../../Simulation/MathUtils.h"
#include "../../Simulation/TestClasses/TestPoint.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <cmath>
#include <iostream>
#include <SDL3/SDL_render.h>
#include <vector>
#include "fonts/font.h"

namespace ChunkNeighborsVisualizer {
    int range{ 130 };
    int mode{ 2 };
    int active_points{ 0 };
    int active_moving_points{ 0 };
    enum class DisplayType {POINT_MAP, DENSITY_MAP};
    DisplayType display_type = DisplayType::DENSITY_MAP;
    SDL_FRect rectT;

    void present_chunk_grid(SDL_Renderer* &renderer);

    void present_chunk_neighbors_filled(SDL_Renderer* &renderer, float mouse_x, float mouse_y, float mouseScrollY);

    void present_closest_connections_mouse(SDL_Renderer* &renderer, float mouse_x, float mouse_y, float mouseScrollY);

    void handle_chunk_input(SDL_Event* e) {
        if (e->type == SDL_EVENT_KEY_DOWN) {
            std::cout<<"Detected key";
            switch (e->key.scancode) {
                case SDL_SCANCODE_0: {
                    mode = 0;
                    break;
                }
                case SDL_SCANCODE_1 : {
                    mode = 1;
                    break;
                }
                case SDL_SCANCODE_2 : {
                    mode = 2;
                    break;
                }
                case SDL_SCANCODE_TAB : {
                    if (display_type == DisplayType::DENSITY_MAP) {
                        display_type = DisplayType::POINT_MAP;
                    }
                    else {
                        display_type = DisplayType::DENSITY_MAP;
                    }
                }
            }
        }
    }

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
        SDL_Texture* texture = font::load_font(renderer);
        std::string mode_text = "Mode: " + std::to_string(mode);
        font::draw_text(renderer, mode_text.c_str(), 660, 20, 2);
        mode_text = "Points moving: " + std::to_string(active_moving_points);
        font::draw_text(renderer, mode_text.c_str(), 660, 40, 2);
        mode_text = "Points non-moving: " + std::to_string(active_points);
        font::draw_text(renderer, mode_text.c_str(), 660, 60, 2);
    }

    void present_closest_connections_mouse(SDL_Renderer* &renderer, float mouse_x, float mouse_y, float mouseScrollY) { // finds the closest connections to agents within the range of the mouse cursor
        range += mouseScrollY * 10.0f;
        range = std::round(range / 10.0f) * 10.0f;

        int flooredX = std::floor(mouse_x/WORLD_CHUNK_SIZE);
        int flooredY = std::floor(mouse_y/WORLD_CHUNK_SIZE);

        bool found_chunk{ false };
        for (int x = 0; x < World::world_map_size; x++) {
            for (int y = 0; y < World::world_map_size; y++) {
                if (flooredX == x && flooredY == y) {
                    found_chunk = true;
                    TestPoint::test_point throw_away;
                    TestPoint::test_point point = MathUtils::find_nearest_connection_test_points(World::chunks_map_main[x][y], mouse_x, mouse_y, range, throw_away);
                    SDL_SetRenderDrawColor(renderer, 255,255,255,255);
                    SDL_RenderLine(renderer, mouse_x, mouse_y, point.x, point.y);
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
        int flooredX = std::floor(mouse_x/WORLD_CHUNK_SIZE);
        int flooredY = std::floor(mouse_y/WORLD_CHUNK_SIZE);
        range += mouseScrollY * 10.0f;
        range = std::round(range / 10.0f) * 10.0f;
        for (int x = 0; x < World::world_map_size; x++) {
            for (int y = 0; y < World::world_map_size; y++) {
                World::chunks_map_main[x][y].visuals.filled = false;
            }
        }
        for (int x = 0; x < World::world_map_size; x++) {
            for (int y = 0; y < World::world_map_size; y++) {
                if (flooredX == x && flooredY == y) {
                    found_chunk = true;
                    for (int &chunk_id : World::chunks_map_main[x][y].cached_chunks[range].chunks) {
                        World::chunks_map_main[(chunk_id - 1) % World::world_map_size][static_cast<int>(std::floor((chunk_id-1)/World::world_map_size))].visuals.filled = true;
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

        active_moving_points = 0;
        active_points = 0;

        constexpr float kCellSize = WORLD_CHUNK_SIZE;

        for (int x = 0; x < World::world_map_size; x++) {
            for (int y = 0; y < World::world_map_size; y++) {
                SDL_FRect rect {x * kCellSize, y * kCellSize, kCellSize, kCellSize};
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                if (display_type == DisplayType::DENSITY_MAP) {
                    Uint8 alpha = static_cast<Uint8>(std::min(255.0f, World::chunks_map_main[x][y].test_points.size() * 0.9f));
                    std::cout<<std::to_string(alpha)<<std::endl;
                    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                    SDL_SetRenderDrawColor(renderer, 0, 0, 255, alpha);
                    World::chunks_map_main[x][y].visuals.filled = true;
                }
                if (World::chunks_map_main[x][y].visuals.filled && mode != 1) {
                    SDL_RenderFillRect(renderer, &rect);
                }
                else {
                    SDL_RenderRect(renderer, &rect);
                }
                if (display_type == DisplayType::POINT_MAP) {
                    for (int i = 0; i < World::chunks_map_main[x][y].test_points.size(); i++) {
                        TestPoint::test_point &point = World::chunks_map_main[x][y].test_points[i];
                        rectT = {point.x, point.y, 5, 5};

                        // Render the movement direction
                        if (display_test_targets && point.moving && point.targetX != point.x && point.targetY != point.y) {
                            SDL_SetRenderDrawColor(renderer,255,255,255,255);
                            SDL_RenderLine(renderer, point.x, point.y, point.targetX, point.targetY);
                        }

                        // Set color then render the point
                        if (point.moving) {
                            active_moving_points += 1;
                            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                        }
                        else {
                            active_points += 1;
                            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                        }
                        SDL_RenderFillRect(renderer, &rectT);
                    }
                }
            }
        }
    }
} // ChunkNeighborsVisualizer