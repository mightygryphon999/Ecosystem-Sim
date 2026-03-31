cmake_minimum_required(VERSION 3.22)
project(EcosystemSimulator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_C_COMPILER "/usr/bin/clang")

add_executable(${PROJECT_NAME} src/main.cpp
        src/Simulation/Chunk.cpp
        src/Simulation/Chunk.h
        src/Simulation/World.cpp
        src/Simulation/World.h
        src/Rendering/Renderer.cpp
        src/Rendering/Renderer.h
)

set(SDL3_ROOT "/opt/homebrew/opt/sdl3")

target_include_directories(${PROJECT_NAME} PRIVATE ${SDL3_ROOT}/include)

target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL3_ROOT}/lib/libSDL3.dylib)

if(APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY})
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
        BUILD_WITH_INSTALL_RPATH TRUE
        INSTALL_RPATH "${SDL3_ROOT}/lib"
)