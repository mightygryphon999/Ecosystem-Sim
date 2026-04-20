//
// Created by Griffin Roberts on 4/14/26.
//

#ifndef C__VERSION_OPENGLCOMPUTEHANDLER_H
#define C__VERSION_OPENGLCOMPUTEHANDLER_H

#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include "../../../../include/glad/glad.h"
#include "../../../Simulation/World.h"

namespace OpenGLComputeHandler {

class OpenGLComputeHandler {
    const int N = WORLD_SIZE; // chunk amount;
    const int WG = 256; // local size x in GLSL

    GLuint prog = 0;
    GLint locDT = 0;
    GLint locCount = 0;

    GLuint chunks;
    GLuint cached_chunks;
    GLuint chunk_indices;
    GLuint test_points;
    GLuint max_chunk_search_range;
    GLuint movement_amount;
    GLuint movement_step;
    GLuint moved_count;

    uint max_chunk_search_range_value = 910;
    uint movement_amount_value = 1;
    uint movement_step_value = 1;
    uint moved_count_value = 0;
    // note for future: implement an SSBO value with GLuint for every buffer value

    GLuint createSSBO(const void *data, GLsizeiptr bytes, GLuint bindpoint);

    void startCompute();
    void runCompute();
    void deleteCompute();
    void generate_scalar_buffer();
};

} // OpenGLComputeHandler

#endif //C__VERSION_OPENGLCOMPUTEHANDLER_H
