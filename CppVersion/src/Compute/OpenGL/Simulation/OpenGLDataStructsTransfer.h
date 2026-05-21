//
// Created by Griffin Roberts on 4/16/26.
//

#ifndef C__VERSION_OPENGLDATASTRUCTSTRANSFER_H
#define C__VERSION_OPENGLDATASTRUCTSTRANSFER_H
#include <sys/types.h>
#include <vector>

namespace OpenGLDataStructsTransfer {

    // For encoding the data
    struct ChunkE {
        float x;
        float y;
        float Cw;
        int id;

        uint cached_start;
        uint cached_count;

        uint test_points_start;
        uint test_points_count;
    };

    struct CachedEntry {
        int key;
        uint start;
        uint count;
    };

    struct TestPointCompute{
        float x;
        float y;
        float range;
        int moving;
        uint trash;
        int chunk_id;
    };

    inline std::vector<ChunkE> chunks;
    inline std::vector<CachedEntry> cached_chunks;
    inline std::vector<int> chunk_indices;
    inline std::vector<TestPointCompute> test_points;

    void setup_data_test_points();

    // For decoding the data




} // OpenGLDataStructsTransfer

#endif //C__VERSION_OPENGLDATASTRUCTSTRANSFER_H
