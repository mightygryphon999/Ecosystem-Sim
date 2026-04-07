//
// Created by Griffin Roberts on 3/30/26.
//

#ifndef C__VERSION_WORLD_H
#define C__VERSION_WORLD_H
#include "Chunk.h"
#include "../config.h"

namespace World {
    constexpr int world_map_size = WORLD_SIZE / WORLD_CHUNK_SIZE;

    extern ChunkData::Chunk chunks_map_main[world_map_size][world_map_size];

    int init(bool testing, int testingMovingAmount);

    int simulation_step(bool testing);
} // World

#endif //C__VERSION_WORLD_H
