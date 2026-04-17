//
// Created by Griffin Roberts on 4/16/26.
//

#include "OpenGLDataStructsTransfer.h"
#include "../../../Simulation/World.h"
#include "../../../Simulation/Chunk.h"

namespace OpenGLDataStructsTransfer {
    std::vector<Chunk> chunks;
    std::vector<CachedEntry> cached_chunks;
    std::vector<int> chunk_indices;
    std::vector<TestPoint> test_points;

    void setup_data() {
        for (int x = 0; x < World::world_map_size; x++) {
            for (int y = 0; y < World::world_map_size; y++) {
                ChunkData::Chunk c = World::chunks_map_main[x][y];
            }
        }
    }
} // OpenGLDataStructsTransfer