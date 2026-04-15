//
// Created by Griffin Roberts on 4/14/26.
//
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include "../../../../include/glad/glad.h"

#include "OpenGLComputeHandler.h"

namespace OpenGLComputeHandler {
    std::string loadFile(const std::string& path) {
        std::ifstream f(path);
        if (!f) throw std::runtime_error("Cannot open: " + path);
        std::ostringstream ss; ss << f.rdbuf();
        return ss.str();
    }

    GLuint createComputeProgram(const std::string& src) {
        GLuint shader = glCreateShader(GL_COMPUTE_SHADER);
        const char* cstr = src.c_str();
        glShaderSource(shader, 1, &cstr, nullptr);
        glCompileShader(shader);

        GLint ok = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
        if (!ok) {
            char log[1024]; glGetShaderInfoLog(shader,sizeof(log), nullptr, log);
            glDeleteShader(shader);
            throw std::runtime_error(std::string("Compile Error:\n") + log);
        }

        GLuint prog = glCreateProgram();
        glAttachShader(prog, shader);
        glLinkProgram(prog);

        glGetProgramiv(prog, GL_LINK_STATUS, &ok);
        if (!ok) {
            char log[1024]; glGetProgramInfoLog(prog,sizeof(log), nullptr, log);
            glDeleteProgram(prog);
            throw std::runtime_error(std::string("Link Error:\n") + log);
        }
        glDetachShader(prog, shader);
        glDeleteShader(shader);
        return prog;
    }
    GLuint createSSBO(const void* data, GLsizeiptr bytes, GLuint bindpoint) {
        GLuint buf;
        glGenBuffers(1, &buf);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, buf);
        glBufferData(GL_SHADER_STORAGE_BUFFER, bytes, data, GL_DYNAMIC_COPY);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindpoint, buf);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
        return buf;
    }
    void OpenGLComputeHandler::startCompute() {
        prog = createComputeProgram(loadFile("simulation_compute.comp"));

        locDT = glGetUniformLocation(prog, "uDeltaTime");
        locCount = glGetUniformLocation(prog, "uCount");
    }

    void OpenGLComputeHandler::runCompute() {
        glUseProgram(prog);
        glUniform1ui(locCount, N);

        glDispatchCompute((N+WG - 1) / WG, 1, 1);

        // use glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, (ssbo name)) for adding the data

        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    }

    void OpenGLComputeHandler::deleteCompute() {
        glDeleteBuffers(); // input names and such into this starting with 1, name
        glDeleteProgram(prog);
    }
}
