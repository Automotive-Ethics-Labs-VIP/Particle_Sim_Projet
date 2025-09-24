cmake_minimum_required(VERSION 3.10)
project(ParticleSimDemo)

set(CMAKE_CXX_STANDARD 17)

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
)

# Create executable
add_executable(particle_demo ${SOURCES})

# Link libraries
target_link_libraries(particle_demo glfw OpenGL::GL)

# Copy shaders to build directory
file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})
