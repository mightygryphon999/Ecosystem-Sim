//
// Created by Griffin Roberts on 3/30/26.
//

#ifndef C__VERSION_WORLD_H
#define C__VERSION_WORLD_H
#include "Chunk.h"

namespace World {
    extern ChunkData::Chunk chunks_map_main[10][10];

    int init();

    int simulation_step(bool testing);
} // World

#endif //C__VERSION_WORLD_H
