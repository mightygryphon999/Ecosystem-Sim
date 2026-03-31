//
// Created by Griffin Roberts on 3/30/26.
//

#ifndef C__VERSION_CHUNK_H
#define C__VERSION_CHUNK_H
#include "../Rendering/ChunkVisual.h"
#include <unordered_map>
#include <vector>

namespace ChunkData {

    struct Chunk;

    struct chunk_list{
        std::vector<Chunk*> chunks;
    };

    struct Chunk {
        float x,y;
        float Cw;
        std::unordered_map<int,chunk_list> cached_chunks;
        ChunkVisual::ChunkVisual visuals;

        void _init(float y_pos, float x_pos, float width);

        int _setup_cached_chunks(int max_range, int step, Chunk (&chunks_map)[10][10]);
    };
} // Chunk

#endif //C__VERSION_CHUNK_H
