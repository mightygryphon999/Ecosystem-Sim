//
// Created by Griffin Roberts on 3/30/26.
//

#include "World.h"

#include "Chunk.h"

namespace World {
    int setup_chunks(ChunkData::Chunk (&chunks_map)[10][10]){
        int i = 0;
        int j = 0;

        for (i = 0; i < 10; i++) {
            for (j = 0; j < 10; j++) {
                const ChunkData::Chunk c = {static_cast<float>(j) * 64,static_cast<float>(i) * 64,64};
                chunks_map[i][j] = c;
            }
        }
        return 1;
    }

    int setup_cached_chunks(){
        for (int x = 0; x < 10; x++) {
            for (int y = 0; y < 10; y++) {
                chunks_map_main[x][y]._setup_cached_chunks(250, 10, chunks_map_main);
            }
        }
        return 1;
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
