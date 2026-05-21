//
// Created by Griffin Roberts on 4/14/26.
//
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include "../../../../include/glad/glad.h"
#include "OpenGLDataStructsTransfer.h"

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
    GLuint OpenGLComputeHandler::createSSBO(const void* data, GLsizeiptr bytes, GLuint bindpoint) {
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

        glUseProgram(prog);
        chunks = createSSBO(nullptr, 500000000, 0);
        cached_chunks = createSSBO(nullptr, 500000000, 1);
        chunk_indices = createSSBO(nullptr, 500000000, 2);
        test_points = createSSBO(nullptr, 500000000, 3);

        glGenBuffers(1, &movement_amount);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, movement_amount);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(uint), &movement_amount_value, GL_STATIC_COPY);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, movement_amount);

        glGenBuffers(1, &max_chunk_search_range);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, max_chunk_search_range);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(uint), &max_chunk_search_range_value, GL_STATIC_COPY);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, max_chunk_search_range);

        glGenBuffers(1, &movement_step);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, movement_step);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(uint), &movement_step_value, GL_STATIC_COPY);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, movement_step);

        glGenBuffers(1, &moved_count);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, moved_count);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(uint), &moved_count_value, GL_STATIC_COPY);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 8, moved_count);
    }

    void OpenGLComputeHandler::runCompute() {
        glUseProgram(prog);
        glUniform1ui(locCount, N);

        OpenGLDataStructsTransfer::setup_data_test_points();

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, chunks);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, OpenGLDataStructsTransfer::chunks.size() * sizeof(OpenGLDataStructsTransfer::ChunkE), OpenGLDataStructsTransfer::chunks.data());

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, chunks);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 1, OpenGLDataStructsTransfer::cached_chunks.size() * sizeof(OpenGLDataStructsTransfer::CachedEntry), OpenGLDataStructsTransfer::cached_chunks.data());

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, chunks);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 2, OpenGLDataStructsTransfer::chunk_indices.size() * sizeof(int), OpenGLDataStructsTransfer::chunk_indices.data());

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, chunks);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 3, OpenGLDataStructsTransfer::test_points.size() * sizeof(OpenGLDataStructsTransfer::TestPointCompute), OpenGLDataStructsTransfer::test_points.data());

        uint zero = 0;
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, moved_count);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(uint), &zero);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, max_chunk_search_range);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, movement_amount);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, movement_step);

        glDispatchCompute((N+WG - 1) / WG, 1, 1);

        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    }

    void OpenGLComputeHandler::deleteCompute() {
        glDeleteBuffers(1, &chunks); // input names and such into this starting with 1, name
        glDeleteBuffers(1, &cached_chunks);
        glDeleteBuffers(1, &chunk_indices);
        glDeleteBuffers(1, &test_points);
        glDeleteBuffers(1, &max_chunk_search_range);
        glDeleteBuffers(1, &movement_amount);
        glDeleteBuffers(1, &movement_step);

        glDeleteProgram(prog);
    }
}
