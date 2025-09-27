cmake_minimum_required(VERSION 3.10)
project(ParticleSimulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

# Include directories
include_directories(src)

# Source files
set(SOURCES
    src/main.cpp
    src/particle/Particle.cpp
    src/particle/ParticleSystem.cpp
    src/physics/PhysicsEngine.cpp
    src/rendering/Renderer.cpp
    src/utils/JSONExporter.cpp
    src/utils/PerformanceProfiler.cpp
)

# Create executable
add_executable(particle_simulator ${SOURCES})

# Link libraries
target_link_libraries(particle_simulator glfw OpenGL::GL)

# Compiler flags for optimization
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(particle_simulator PRIVATE -O3 -march=native)
endif()

# Debug flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(particle_simulator PRIVATE -g -Wall -Wextra)
endif()

# Copy shaders to build directory
file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})

# Create output directories
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/output)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/logs)
