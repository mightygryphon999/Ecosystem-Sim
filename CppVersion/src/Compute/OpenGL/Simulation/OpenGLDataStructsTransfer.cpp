//
// Created by Griffin Roberts on 4/16/26.
//

#include "OpenGLDataStructsTransfer.h"
#include "../../../Simulation/World.h"
#include "../../../Simulation/Chunk.h"
#include "../../../Simulation/TestClasses/TestPoint.h"

namespace OpenGLDataStructsTransfer {

    void setup_data_test_points() {
        chunks.clear();
        cached_chunks.clear();
        chunk_indices.clear();
        test_points.clear();

        chunks.reserve(World::world_map_size * World::world_map_size);

        ChunkData::chunk_list cl;
        CachedEntry new_entry = {0,0,0};
        ChunkData::Chunk c;
        TestPointCompute p = {0,0,0,0};
        Chunk new_chunk = {0,0,0,0,0,0,0,0};
        TestPoint::test_point tp = {0,0,0,0,0,0};

        for (int x = 0; x < World::world_map_size; x++) {
            for (int y = 0; y < World::world_map_size; y++) {
                new_chunk = {};

                c = World::chunks_map_main[x][y];;

                int id = c.id;

                new_chunk.id = id;
                new_chunk.x = c.x;
                new_chunk.y = c.y;
                new_chunk.Cw = c.Cw;

                new_chunk.cached_start = static_cast<uint>(cached_chunks.size());
                new_chunk.cached_count = static_cast<uint>(c.cached_chunks.size());

                new_chunk.test_points_start = static_cast<uint>(test_points.size());
                new_chunk.test_points_count = static_cast<uint>(c.test_points.size());

                for (int chunk_neighbor_range_index = 0; chunk_neighbor_range_index < c.cached_chunks.size(); chunk_neighbor_range_index++) {
                    cl = c.cached_chunks[chunk_neighbor_range_index];

                    new_entry.key = chunk_neighbor_range_index;

                    new_entry.start = chunk_indices.size();
                    new_entry.count = cl.chunks.size();

                    chunk_indices.insert(chunk_indices.end(), cl.chunks.begin(), cl.chunks.end());

                    cached_chunks.push_back(new_entry);
                }

                for (int test_point_index = 0; test_point_index < c.test_points.size(); test_point_index++) {
                    tp = c.test_points[test_point_index];

                    if (tp.moving) {
                        p.moving = 1;
                    } else {
                        p.moving = 0;
                    }

                    p.x = tp.x;
                    p.y = tp.y;

                    p.range = 200;

                    p.chunk_id = new_chunk.id;

                    p.trash = 0;

                    test_points.push_back(p);
                }

                chunks.push_back(new_chunk);
            }
        }
    }
    void decode_data_test_points() {
        // use this to read and update the CPU side simulation
        // when running the GPU make it race ahead of the CPU for slightly faster sim times, have it do the next simulation step so that they CPU can use the original data dna the GPU uses a wrong but new data

    }
} // OpenGLDataStructsTransfer