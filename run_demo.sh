#!/bin/bash

echo "=== Team B Demo Build Script ==="

# Create build directory
mkdir -p build
cd build

# Configure with CMake
echo "Configuring with CMake..."
cmake ..

# Build the project
echo "Building project..."
make

# Check if build was successful
if [ $? -eq 0 ]; then
    echo "Build successful! Running demo..."
    ./particle_demo
else
    echo "Build failed. Check dependencies:"
    echo "  - GLM (OpenGL Mathematics)"
    echo "  - GLFW3"
    echo "  - OpenGL development libraries"
    echo ""
    echo "Ubuntu/Debian install: sudo apt-get install libglfw3-dev libglm-dev libgl1-mesa-dev"
    echo "macOS install: brew install glfw glm"
fi
