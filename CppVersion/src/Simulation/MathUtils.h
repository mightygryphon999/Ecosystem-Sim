//
// Created by Griffin Roberts on 3/31/26.
//

#ifndef C__VERSION_MATHUTILS_H
#define C__VERSION_MATHUTILS_H
#include "Chunk.h"
#include "TestClasses/TestPoint.h"
#include <utility>

namespace MathUtils {
    TestPoint::test_point find_nearest_connection_test_points(ChunkData::Chunk &parent_chunk, float pos_x, float pos_y, float range, TestPoint::test_point &point);

    std::pair<float,float> move_towards(float x1, float y1, float x2, float y2, float move_amount, float step);
} // MathUtils

#endif //C__VERSION_MATHUTILS_H
