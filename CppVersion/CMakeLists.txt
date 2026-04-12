cmake_minimum_required(VERSION 3.22)
project(EcosystemSimulator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_SCAN_FOR_MODULES ON)

set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm/bin/clang++")
set(CMAKE_C_COMPILER "/opt/homebrew/opt/llvm/bin/clang")

add_executable(${PROJECT_NAME} src/main.cpp
        src/Simulation/Chunk.cpp
        src/Simulation/Chunk.h
        src/Simulation/World.cpp
        src/Simulation/World.h
        src/Rendering/Renderer.cpp
        src/Rendering/Renderer.h
        src/Rendering/ChunkVisual.cpp
        src/Rendering/ChunkVisual.h
        src/Rendering/Visualizers/ChunkNeighborsVisualizer.cpp
        src/Rendering/Visualizers/ChunkNeighborsVisualizer.h
        src/Simulation/TestClasses/TestPoint.cpp
        src/Simulation/TestClasses/TestPoint.h
        src/Simulation/MathUtils.cpp
        src/Simulation/MathUtils.h
        src/Rendering/Visualizers/fonts/font_bitmap.h
        src/Rendering/Visualizers/fonts/font.cpp
        src/Rendering/Visualizers/fonts/font.h
        src/Rendering/SimulationInteractions.cpp
        src/Rendering/SimulationInteractions.h
        src/config.h
        src/Rendering/UI/UIElements/UICheckbox.h
        src/Rendering/UI/UIEditorSettings/UIEditorSettingsObjects/UIViewTargetLineCheckbox.cpp
        src/Rendering/UI/UIEditorSettings/UIEditorSettingsObjects/UIViewTargetLineCheckbox.h
        src/Rendering/UI/UIEditorSettings/UIEditorSettingsRenderer.cpp
        src/Rendering/UI/UIEditorSettings/UIEditorSettingsRenderer.h
        src/Rendering/UI/UIElements/UIVerticalElement.h
        src/Rendering/UI/UIElements/LayoutElements/UIVerticalLayout.cpp
        src/Rendering/UI/UIElements/LayoutElements/UIVerticalLayout.h
        src/Rendering/UI/UIElements/UIElement.h
        src/Compute/Simulation/SimVulkanHandler.cpp
        src/Compute/Simulation/SimVulkanHandler.h
        src/Compute/Simulation/SimVulkanHandler.cpp
        src/Compute/Simulation/SimVulkanHandler.cpp
        src/Rendering/VulkanRendering/VulkanBackend.cpp
        src/Rendering/VulkanRendering/VulkanBackend.h
)

set(SDL3_ROOT "/opt/homebrew/opt/sdl3")

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

target_include_directories(${PROJECT_NAME} PRIVATE ${SDL3_ROOT}/include)

target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL3_ROOT}/lib/libSDL3.dylib)

target_include_directories(${PROJECT_NAME} PRIVATE "${Vulkan_INCLUDE_DIR}")

add_library(VulkanCppModule)
add_library(Vulkan::cppm ALIAS VulkanCppModule)

target_compile_definitions(VulkanCppModule PUBLIC
        VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1
        VULKAN_HPP_NO_STRUCT_CONSTRUCTORS=1
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
        VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1
        VULKAN_HPP_NO_STRUCT_CONSTRUCTORS=1
)

target_include_directories(VulkanCppModule PRIVATE "${Vulkan_INCLUDE_DIR}")

target_link_libraries(VulkanCppModule PUBLIC Vulkan::Vulkan)

set_target_properties(VulkanCppModule PROPERTIES CXX_STANDARD 20)

target_sources(VulkanCppModule
        PUBLIC
        FILE_SET cxx_modules TYPE CXX_MODULES
        BASE_DIRS "${Vulkan_INCLUDE_DIR}"
        FILES "${Vulkan_INCLUDE_DIR}/vulkan/vulkan.cppm"
)

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