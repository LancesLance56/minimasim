cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER "C:/Program Files/w64devkit/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/Program Files/w64devkit/bin/g++.exe")

project(engine)

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DGLFW_INCLUDE_NONE)

set(CMAKE_BUILD_TYPE Debug)


# Set include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/gfx
        ${CMAKE_SOURCE_DIR}/include/imgui
        ${CMAKE_SOURCE_DIR}/include/imgui/backends
)

# Add source files
set(SOURCES
    include/glad.c
    include/imgui/imgui.cpp
    include/imgui/imgui.h
    include/imgui/imgui_demo.cpp
    include/imgui/imgui_draw.cpp
    include/imgui/imgui_tables.cpp
    include/imgui/imgui_widgets.cpp
    include/imgui/backends/imgui_impl_glfw.cpp
    include/imgui/backends/imgui_impl_opengl3.cpp
    src/main.cpp
        src/app/engine_app.cpp
        src/engine/engine.cpp
        src/engine/engine_gui.cpp
        src/engine/precompiled_entities.cpp
        src/engine/precompiled_shaders.cpp
        src/engine/entity/components/mesh_filter_component.cpp
            src/engine/entity/components/mesh_renderer_component.cpp
            src/engine/entity/components/trail_component.cpp
        src/gfx/bo.cpp
        src/gfx/camera.cpp
        src/gfx/light.cpp
        src/gfx/line_strip_utils.cpp
        src/gfx/material.cpp
        src/gfx/shader.cpp
        src/gfx/texture.cpp
        src/gfx/vao.cpp
        src/gfx/window.cpp
        src/imgui/animation_curve.cpp
        src/imgui/imgui_helper.cpp
        src/input/handle_input.cpp
        src/math/bezier.cpp
        src/math/n_body_sim.cpp
        src/mesh/axis_entity.cpp
        src/mesh/cube_entity.cpp
        src/mesh/sphere_entity.cpp
        src/scenes/scene_gravity.cpp
)

# Add executable target
add_executable(engine ${SOURCES})

# Link GLFW library
target_link_libraries(engine ${CMAKE_SOURCE_DIR}/lib/libglfw3dll.a)
