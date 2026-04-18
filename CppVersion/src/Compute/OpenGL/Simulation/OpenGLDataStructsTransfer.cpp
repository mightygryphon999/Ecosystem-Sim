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

                chunks.push_back(Chunk{c.x, c.y, c.Cw, c.id, 0, 0});
                int id = chunks.size() - 1;

                for (int chunk_neighbor_range_index = 0; chunk_neighbor_range_index < c.cached_chunks.size(); chunk_neighbor_range_index++) {
                    
                }
            }
        }
    }
} // OpenGLDataStructsTransfer