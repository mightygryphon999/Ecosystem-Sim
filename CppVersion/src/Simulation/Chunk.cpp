//
// Created by Griffin Roberts on 3/30/26.
//

#include "Chunk.h"
#include "../Simulation/MathUtils.h"
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <iostream>
#include <unordered_set>
#include <utility>
#include "../config.h"
#include "World.h"
#include "../Rendering/ChunkVisual.h"

namespace ChunkData {
	void Chunk::_init(float y_pos, float x_pos, float width, int id_new) {
		x = x_pos;
		y = y_pos;
		Cw = width;
		id = id_new;
	}
	int Chunk::_setup_cached_chunks(int max_range, int step, Chunk (&chunks_map)[world_map_size_chunks][world_map_size_chunks]){
	    int i = 0;
		float Cx = this->x + this->Cw/2;
		float Cy = this->y + this->Cw/2;

		std::unordered_set<int> chunks_already_added;

	    for (i = 0; i < max_range; i += step){
	        int range = i;
	        for (int x_new = 0; x_new < world_map_size_chunks; x_new++){
	            for (int y_new = 0; y_new < world_map_size_chunks; y_new++){
	                Chunk &testing_chunk = chunks_map[x_new][y_new];

	            	if (chunks_already_added.contains(testing_chunk.id)) continue;

	                float Rx = testing_chunk.x;
	                float Ry = testing_chunk.y;
	                const float Rw = testing_chunk.Cw;
	                const int R = range;
	                const float closestX = std::clamp(Cx, Rx, Rx + Rw);
	                const float closestY = std::clamp(Cy, Ry, Ry + Rw);
	                const float distanceX = Cx - closestX;
	                const float distanceY = Cy - closestY;
	                const float distanceSquared = (distanceX*distanceX)+(distanceY*distanceY);
	                const float radiusSquared = R*R;
	                if (distanceSquared <= radiusSquared){
	                    cached_chunks[range].chunks.push_back(testing_chunk.id);
	                	chunks_already_added.insert(testing_chunk.id);
	                }
	            }
	        }
	    }
		return 1;
	}
	int Chunk::_simulation_step(bool testing) {
		if (testing) {
			constexpr float range = 250;
			for (int i = 0; i < test_points.size(); i++) {
				TestPoint::test_point &point = test_points[i];
				if (point.moving) {
					TestPoint::test_point target = MathUtils::find_nearest_connection_test_points(*this, point.x, point.y, range, point);
					std::pair<float, float> new_pos = MathUtils::move_towards(point.x,point.y,target.x,target.y,1,1);
					point.x = new_pos.first;
					point.y = new_pos.second;
				}
			}
		}
		return 0;
	}
}