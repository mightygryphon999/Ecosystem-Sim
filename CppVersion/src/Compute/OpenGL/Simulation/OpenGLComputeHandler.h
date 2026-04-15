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

    // note for future: implement an SSBO value with GLuint for every buffer value

    void startCompute();
    void runCompute();
};

} // OpenGLComputeHandler

#endif //C__VERSION_OPENGLCOMPUTEHANDLER_H
