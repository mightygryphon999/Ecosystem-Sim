//
// Created by Griffin Roberts on 3/31/26.
//

#ifndef C__VERSION_CHUNKNEIGHBORSVISUALIZER_H
#define C__VERSION_CHUNKNEIGHBORSVISUALIZER_H
#include <SDL3/SDL_render.h>

namespace ChunkNeighborsVisualizer {
    void present_chunk_neighbors_visualizer(SDL_Renderer* &renderer, float mouse_x, float mouse_y, float mouseScrollY);
} // ChunkNeighborsVisualizer

#endif //C__VERSION_CHUNKNEIGHBORSVISUALIZER_H
