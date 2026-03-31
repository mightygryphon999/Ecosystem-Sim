//
// Created by Griffin Roberts on 3/31/26.
//

#include "ChunkVisual.h"

namespace ChunkVisual {
    struct ChunkVisual {
        bool filled{ false };
        int Outline[4] = {255,255,255,1};
        int Filled[4] = {0,255,0,1};
    };
}