//
// Created by Griffin Roberts on 3/31/26.
//

#ifndef C__VERSION_MATHUTILS_H
#define C__VERSION_MATHUTILS_H
#include "Chunk.h"
#include "TestClasses/TestPoint.h"

namespace MathUtils {
    TestPoint::test_point find_nearest_connection_test_points(const ChunkData::Chunk &parent_chunk, float pos_x, float pos_y, float range);
} // MathUtils

#endif //C__VERSION_MATHUTILS_H
