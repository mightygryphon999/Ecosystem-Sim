//
// Created by Griffin Roberts on 3/30/26.
//

#include "World.h"
#include <iostream>
#include "Chunk.h"
#include <random>
#include "../config.h"

namespace World {

    ChunkData::Chunk chunks_map_main[world_map_size][world_map_size];

    int setup_chunks(ChunkData::Chunk (&chunks_map)[world_map_size][world_map_size]){
        int i = 0;
        int j = 0;

        unsigned seed = static_cast<unsigned int>(std::time(nullptr));

        std::mt19937 rng(seed);
        std::uniform_int_distribution<int> dist(0, 1);

        int test_points_id{ 0 };

        for (i = 0; i < world_map_size; i++) {
            for (j = 0; j < world_map_size; j++) {
                const ChunkData::Chunk c = {static_cast<float>(i) * WORLD_CHUNK_SIZE,static_cast<float>(j) * WORLD_CHUNK_SIZE, WORLD_CHUNK_SIZE};
                chunks_map[i][j] = c;

                // for spawning test points randomly
                if (dist(rng) == 1) {
                    chunks_map[i][j].test_points.push_back({chunks_map[i][j].x + chunks_map[i][j].Cw/2,chunks_map[i][j].y + chunks_map[i][j].Cw/2,false, test_points_id});
                }
                if (dist(rng) == 0) {
                    chunks_map[i][j].test_points.push_back({chunks_map[i][j].x + chunks_map[i][j].Cw/2,chunks_map[i][j].y + chunks_map[i][j].Cw/2,true, test_points_id});
                }

                test_points_id++;
            }
        }
        return 1;
    }

    int setup_cached_chunks(){
        for (int x = 0; x < world_map_size; x++) {
            for (int y = 0; y < world_map_size; y++) {
                chunks_map_main[x][y]._setup_cached_chunks(250, 10, chunks_map_main);
            }
        }
        return 1;
    }

    int simulation_step(bool testing) {
        std::cout << "SIM chunks_map_main address: " << &chunks_map_main << std::endl;

        for (int x =  0; x < world_map_size; x++) {
            for (int y = 0; y < world_map_size; y++) {
                chunks_map_main[x][y]._simulation_step(testing);
            }
        }

        //deal with transfer of test points
        for (int x = 0; x < world_map_size; x++) {
            for (int y = 0; y < world_map_size; y++) {
                auto& points = chunks_map_main[x][y].test_points;
                for (int i = points.size() - 1; i >= 0; i--) {
                    int new_cx = std::floor(points[i].x / WORLD_CHUNK_SIZE);
                    int new_cy = std::floor(points[i].y / WORLD_CHUNK_SIZE);

                    new_cx = std::clamp(new_cx, 0, world_map_size - 1);
                    new_cy = std::clamp(new_cy, 0, world_map_size - 1);

                    if (new_cx != x || new_cy != y) {
                        chunks_map_main[new_cx][new_cy].test_points.push_back(points[i]);
                        points.erase(points.begin() + i);
                    }
                }
            }
        }
    }

    int init()
    {
        int success = setup_chunks(chunks_map_main);

        if (!success) {
            return 1;
        }
        success = setup_cached_chunks();

        if (!success) {
            return 1;
        }

        return 0;
    }
}
