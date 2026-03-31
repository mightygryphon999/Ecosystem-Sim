//
// Created by Griffin Roberts on 3/30/26.
//

#ifndef C__VERSION_WORLD_H
#define C__VERSION_WORLD_H
#include "Chunk.h"

namespace World {
    inline ChunkData::Chunk chunks_map_main[10][10];

    int init();
} // World

#endif //C__VERSION_WORLD_H
