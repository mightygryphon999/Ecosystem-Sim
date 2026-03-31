//
// Created by Griffin Roberts on 3/30/26.
//

#include "World.h"

#include "Chunk.h"

namespace World {
    ChunkData::Chunk chunks_map[10][10];

    int setup_chunks(ChunkData::Chunk (&chunks_map)[10][10]){
        int i = 0;
        int j = 0;

        for (i = 0; i < 10; i++) {
            for (j = 0; j < 10; j++) {
                const ChunkData::Chunk c = {static_cast<float>(j),static_cast<float>(i),1};
                chunks_map[i][j] = c;
            }
        }
        return 1;
    }

    int setup_cached_chunks(ChunkData::Chunk (&chunks_map)[10][10]){
        return 1;
    }

    int init()
    {
        int success = setup_chunks(chunks_map);

        if (!success) {
            return 1;
        }
        //success = setup_cached_chunks(&chunks_map);

        return 0;
    }
}
