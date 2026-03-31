//
// Created by Griffin Roberts on 3/30/26.
//

#include "Chunk.h"
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <vector>
#include <iostream>
#include <unordered_set>

#include "../Rendering/ChunkVisual.h"

namespace ChunkData {
	void Chunk::_init(float y_pos, float x_pos, float width) {
		x = x_pos;
		y = y_pos;
		Cw = width;
	}
	int Chunk::_setup_cached_chunks(int max_range, int step, Chunk (&chunks_map)[10][10]){
	    int i = 0;
		float Cx = this->x + this->Cw/2;
		float Cy = this->y + this->Cw/2;

		std::unordered_set<Chunk*> chunks_already_added;

	    for (i = 0; i < max_range; i += step){
	        int range = i;
	    	std::cout<<"Calling for range: "<<range<<std::endl;
	        for (int x_new = 0; x_new < 10; x_new++){
	            for (int y_new = 0; y_new < 10; y_new++){
	                Chunk &testing_chunk = chunks_map[x_new][y_new];

	            	if (chunks_already_added.count(&testing_chunk)) continue;

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
	                    cached_chunks[range].chunks.push_back(&chunks_map[x_new][y_new]);
	                	chunks_already_added.insert(&testing_chunk);
	                }
	            }
	        }
	    }
		return 1;
	}
}