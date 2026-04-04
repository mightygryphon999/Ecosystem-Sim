//
// Created by Griffin Roberts on 3/31/26.
//

#include "MathUtils.h"
#include "Chunk.h"
#include "TestClasses/TestPoint.h"
#include <utility>
#include "World.h"
#include "../config.h"

namespace MathUtils {

    float distance_between_two_points(float x, float y, float x2, float y2);
    float fast_inv_sqrt(float number);

    TestPoint::test_point find_nearest_connection_test_points(const ChunkData::Chunk &parent_chunk, float pos_x, float pos_y, float range, TestPoint::test_point &point) {
        if (parent_chunk.test_points.size() > 1) {
            float smallest_distance = std::numeric_limits<float>::max();
            int smallest_index = -1;
            for (int i = 0; i < parent_chunk.test_points.size(); i++) {
                TestPoint::test_point point = parent_chunk.test_points[i];
                if ((point.x == pos_x && point.y == pos_y) || point.moving) {
                    continue;
                }
                float distance = distance_between_two_points(parent_chunk.test_points[i].x, parent_chunk.test_points[i].y, pos_x, pos_y);
                if (distance <= range * range && smallest_distance > distance && distance != 0) {
                    smallest_distance = distance;
                    smallest_index = i;
                }
            }
            if (smallest_index != -1) {
                point.targetX = point.x;
                point.targetY = point.y;
                return parent_chunk.test_points[smallest_index];
            }
        }
        TestPoint::test_point best{};
        float smallest_distance = std::numeric_limits<float>::max();
        for (auto& [key, chunk_list] : parent_chunk.cached_chunks) {
            if (key > range) {
                continue;
            }
            for (int b = 0; b < chunk_list.chunks.size(); b++) {
                if (chunk_list.chunks[b]->test_points.size() < 1) {
                    continue;
                }
                for (int i = 0; i < chunk_list.chunks[b]->test_points.size(); i++) {
                    TestPoint::test_point point = chunk_list.chunks[b]->test_points[i];
                    if ((point.x == pos_x && point.y == pos_y) || point.moving) {
                        continue;
                    }
                    float distance = distance_between_two_points(chunk_list.chunks[b]->test_points[i].x, chunk_list.chunks[b]->test_points[i].y, pos_x, pos_y);
                    if (distance <= range * range && smallest_distance > distance) {
                        smallest_distance = distance;
                        best = chunk_list.chunks[b]->test_points[i];
                    }
                }
            }
        }
        point.targetX = best.x;
        point.targetY = best.y;

        return best;
    }

    std::pair<float,float> move_towards(float x1, float y1, float x2, float y2, float move_amount, float step) {
        float dx = x2 - x1;
        float dy = y2 - y1;

        float dist = distance_between_two_points(x1, y1, x2, y2);

        if (dist == 0) {
            return {x1,y1};
        }

        float move_amount_speed = move_amount * step;

        if ((move_amount_speed*move_amount_speed) >= dist) {
            return {x2,y2};
        }

        float invsqr = fast_inv_sqrt(dist);

        float nx = x1 + (dx * invsqr) * move_amount_speed;
        float ny = y1 + (dy * invsqr) * move_amount_speed;

        return {nx, ny};
    }

    float fast_inv_sqrt(float number) {
        int32_t i;
        float x2, y;

        x2 = number*0.5f;
        y = number;

        i = *reinterpret_cast<int32_t *>(&y);
        i = 0x5f3759df - (i >> 1);

        y = *reinterpret_cast<float *>(&i);

        y = y*(1.5f - (x2 *  y * y));

        return y;
    }

    float distance_between_two_points(float x, float y, float x2, float y2) {
        return ((x2-x) * (x2-x))+((y2-y) * (y2-y));
    }
}