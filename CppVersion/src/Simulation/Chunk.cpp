//
// Created by Griffin Roberts on 3/30/26.
//

#include "Chunk.h"
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <vector>
#include <iostream>
#include "../Rendering/ChunkVisual.h"

namespace Chunk {
	struct Chunk;

	struct chunk_list{
	    std::vector<Chunk*> chunks;
	};

	struct Chunk {
		int x,y;
		float Cw;
		std::unordered_map<int,chunk_list> cached_chunks;
		ChunkVisual::ChunkVisual visuals;
		void _init(float y_pos, float x_pos, float width) {
			x = x_pos;
			y = y_pos;
			Cw = width;
		}
		int _setup_cached_chunks(int max_range, int step, Chunk (&chunks_map)[10][10]){
		    int i = 0;
		    for (i = 0; i < max_range; i += step){
		        int range = i;
		        for (x = 0; x < 10; x++){
		            for (y = 0; y < 10; y++){
		                Chunk testing_chunk = chunks_map[x][y];
		                float Rx = testing_chunk.x;
		                float Ry = testing_chunk.y;
		                const float Rw = testing_chunk.Cw;
		                float Cx = static_cast<float>(x) + (static_cast<float>(x)/Cw);
		                float Cy = static_cast<float>(y) + (static_cast<float>(y)/Cw);
		                const int R = range;
		                const float closestX = std::clamp(Cx, Rx, Rx + Rw);
		                const float closestY = std::clamp(Cy, Ry, Ry + Rw);
		                const float distanceX = Cx - closestX;
		                const float distanceY = Cy - closestY;
		                const float distanceSquared = (distanceX*distanceX)+(distanceY*distanceY);
		                const float radiusSquared = std::pow(R, 2);
		                if (distanceSquared <= radiusSquared){
		                    cached_chunks[range].chunks[cached_chunks[range].chunks.size()] = &chunks_map[x][y];
		                }
		            }
		        }
		    }
			return 1;
		}
	};
}