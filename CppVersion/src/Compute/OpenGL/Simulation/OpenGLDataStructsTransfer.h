//
// Created by Griffin Roberts on 4/16/26.
//

#ifndef C__VERSION_OPENGLDATASTRUCTSTRANSFER_H
#define C__VERSION_OPENGLDATASTRUCTSTRANSFER_H

namespace OpenGLDataStructsTransfer {

    struct Chunk {
        float x;
        float y;
        float Cw;
        int id;

        int cached_start;
        int cached_count;

        int test_points_start;
        int test_points_count;
    };

    struct CachedEntry {
        int key;
        int start;
        int end;
    };

    struct TestPoint{
        float x;
        float y;
        float range;
        int moving;
    };
} // OpenGLDataStructsTransfer

#endif //C__VERSION_OPENGLDATASTRUCTSTRANSFER_H
