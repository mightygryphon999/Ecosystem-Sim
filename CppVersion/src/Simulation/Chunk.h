//
// Created by Griffin Roberts on 3/30/26.
//

#ifndef C__VERSION_CHUNK_H
#define C__VERSION_CHUNK_H
#include "../Rendering/ChunkVisual.h"
#include <unordered_map>
#include <vector>
#include "TestClasses/TestPoint.h"
#include "../config.h"

namespace ChunkData {
    constexpr int world_map_size_chunks = WORLD_SIZE / WORLD_CHUNK_SIZE;

    struct Chunk;

    struct chunk_list{
        std::vector<int> chunks;
    };

    struct Chunk {
        float x,y;
        float Cw;
        int id;
        std::unordered_map<int,chunk_list> cached_chunks;
        std::vector<TestPoint::test_point> test_points;
        ChunkVisual::ChunkVisual visuals;

        void _init(float y_pos, float x_pos, float width, int id_new);

        int _setup_cached_chunks(int max_range, int step, Chunk (&chunks_map)[world_map_size_chunks][world_map_size_chunks]);

        int _simulation_step(bool testing);
    };
} // Chunk

#endif //C__VERSION_CHUNK_H
