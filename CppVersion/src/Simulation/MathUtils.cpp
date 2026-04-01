//
// Created by Griffin Roberts on 3/31/26.
//

#include "MathUtils.h"
#include "Chunk.h"
#include "TestClasses/TestPoint.h"
#include <utility>

namespace MathUtils {

    float distance_between_two_points(float x, float y, float x2, float y2);

    TestPoint::test_point find_nearest_connection_test_points(const ChunkData::Chunk &parent_chunk, float pos_x, float pos_y, float range) {
        if (parent_chunk.test_points.size() > 0) {
            float smallest_distance = std::numeric_limits<float>::max();
            int smallest_index = -1;
            for (int i = 0; i < parent_chunk.test_points.size(); i++) {
                TestPoint::test_point point = parent_chunk.test_points[i];
                if (point.x == pos_x && point.y == pos_y) {
                    continue;
                }
                float distance = distance_between_two_points(parent_chunk.test_points[i].x, parent_chunk.test_points[i].y, pos_x, pos_y);
                if (distance <= range * range && smallest_distance > distance && distance != 0) {
                    smallest_distance = distance;
                    smallest_index = i;
                }
            }
            if (smallest_index != -1) {
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
                for (int i = 0; i < chunk_list.chunks[b]->test_points.size(); i++) {
                    TestPoint::test_point point = chunk_list.chunks[b]->test_points[i];
                    if (point.x == pos_x && point.y == pos_y) {
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
        return best;
    }

    std::pair<float,float> move_towards(float x1, float y1, float x2, float y2, float move_amount, float step) {
        float dx = x2 - x1;
        float dy = y2 - y1;

        float dist = std::sqrt(distance_between_two_points(x1, y1, x2, y2));

        if (dist == 0) {
            return {x1,y1};
        }

        float move_amount_speed = std::min(move_amount * step, dist);

        float nx = x1 + (dx / dist) * move_amount_speed;
        float ny = y1 + (dy / dist) * move_amount_speed;

        return {nx, ny};
    }

    float distance_between_two_points(float x, float y, float x2, float y2) {
        return ((x2-x) * (x2-x))+((y2-y) * (y2-y));
    }
}