# Particle Simulator - Team B

## Overview

A real-time particle simulation system built with C++ and OpenGL, demonstrating advanced object-oriented programming principles, physics simulation, and performance optimization techniques. This project provides hands-on experience with simulation libraries, containerization, and high-performance computing concepts.

## Learning Objectives

- **Use C++ in a comprehensive project** with modern programming practices
- **Learn important simulation libraries** including OpenGL for visualization
- **Master performance optimization** techniques and advanced data structures

## Project Features

### Core Simulation
- Real-time particle physics simulation
- Object-oriented particle system architecture
- Advanced force calculations and interactions
- OpenGL-based visualization engine

### Performance Optimization
- Quadtree spatial partitioning for efficient collision detection
- Performance profiling and bottleneck identification
- Advanced data structures for optimal memory usage

## Implementation Steps

1. **OOP Foundation**: Create particle class and simulation loop using C++ principles
2. **Physics Engine**: Implement realistic physics and force interactions
3. **Visualization**: Integrate OpenGL for real-time rendering
4. **Optimization**: Enhance performance using quadtrees and identify bottlenecks

## Must Have Features

- ✅ **Real-time particle simulation** with smooth performance
- ✅ **JSON data export capability** for simulation state persistence
- ✅ **Docker containerization** for consistent deployment
- ✅ **Comprehensive technical documentation**

## Nice to Have Features

- 🎯 **Performance profiling** and optimization metrics (FPS monitoring)
- 🎯 **API endpoint framework** for external communication and control

## Dependencies

### Core Libraries
```
C++17 or higher
OpenGL 3.3+
GLFW (Window management)
GLAD (OpenGL function loading)
GLM (OpenGL Mathematics)
```

### Additional Tools
```
CMake (Build system)
nlohmann/json (JSON handling)
Docker (Containerization)
```

### Optional (Nice to Have)
```
REST API framework (cpp-httplib or similar)
Profiling tools (gprof, Valgrind)
```

## Installation

### Prerequisites
Ensure you have the following installed:
- C++ compiler (GCC 7+ or Clang 5+)
- CMake 3.10+
- Docker
- OpenGL development libraries

### Build Instructions

1. **Clone the repository**:
```bash
git clone [repository-url]
cd particle-simulator
```

2. **Install dependencies** (Ubuntu/Debian):
```bash
sudo apt-get update
sudo apt-get install build-essential cmake
sudo apt-get install libglfw3-dev libglm-dev libgl1-mesa-dev
```

3. **Build the project**:
```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

4. **Run the simulation**:
```bash
./particle_simulator
```

### Docker Deployment

1. **Build Docker image**:
```bash
docker build -t particle-simulator .
```

2. **Run containerized simulation**:
```bash
docker run -p 8080:8080 -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix particle-simulator
```

## Usage

### Basic Simulation
```bash
# Run with default parameters
./particle_simulator

# Run with custom particle count
./particle_simulator --particles 1000

# Export simulation state to JSON
./particle_simulator --export simulation_state.json
```

### API Endpoints (If Implemented)
```
GET /api/status          - Get current simulation status
POST /api/particles      - Add particles to simulation
GET /api/export          - Export current state as JSON
PUT /api/config          - Update simulation parameters
```

## Project Structure

```
particle-simulator/
├── README.md
├── CMakeLists.txt
├── Dockerfile
├── requirements.txt
├── src/
│   ├── main.cpp
│   ├── particle/
│   │   ├── Particle.h
│   │   ├── Particle.cpp
│   │   └── ParticleSystem.cpp
│   ├── physics/
│   │   ├── PhysicsEngine.h
│   │   ├── PhysicsEngine.cpp
│   │   └── Forces.cpp
│   ├── rendering/
│   │   ├── Renderer.h
│   │   ├── Renderer.cpp
│   │   └── Shader.cpp
│   ├── optimization/
│   │   ├── QuadTree.h
│   │   ├── QuadTree.cpp
│   │   └── SpatialHash.cpp
│   └── utils/
│       ├── JSONExporter.h
│       ├── JSONExporter.cpp
│       └── PerformanceProfiler.cpp
├── shaders/
│   ├── vertex.glsl
│   └── fragment.glsl
├── assets/
├── docs/
│   ├── architecture.md
│   ├── performance_analysis.md
│   └── api_documentation.md
├── tests/
└── benchmarks/
```

## Configuration

### Simulation Parameters
```json
{
  "particles": {
    "count": 1000,
    "mass_range": [0.1, 2.0],
    "size_range": [1.0, 5.0]
  },
  "physics": {
    "gravity": -9.81,
    "air_resistance": 0.01,
    "collision_damping": 0.8
  },
  "rendering": {
    "fps_target": 60,
    "window_width": 1280,
    "window_height": 720
  },
  "optimization": {
    "use_quadtree": true,
    "max_quadtree_depth": 8
  }
}
```

## Performance Targets

The simulation aims to achieve:
- **60+ FPS** with 1000+ particles
- **Smooth real-time interaction** with user input
- **Efficient memory usage** through optimized data structures
- **Sub-millisecond** collision detection with spatial partitioning

## Technical Documentation

### Architecture Overview
- **Particle System**: Core simulation engine managing particle lifecycle
- **Physics Engine**: Handles force calculations, collisions, and integration
- **Renderer**: OpenGL-based visualization system
- **Optimization Layer**: Spatial data structures for performance enhancement

### Key Algorithms
- Verlet integration for stable particle physics
- Quadtree spatial partitioning for O(n log n) collision detection
- GPU-accelerated rendering pipeline
- Lock-free data structures for multi-threading

## Development

### Building for Development
```bash
# Debug build with profiling
cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_PROFILING=ON ..
make

# Release build with optimizations
cmake -DCMAKE_BUILD_TYPE=Release -DENABLE_OPTIMIZATIONS=ON ..
make
```

### Testing
```bash
# Run unit tests
make test

# Run performance benchmarks
./benchmarks/performance_test
```

### Profiling
```bash
# CPU profiling with gprof
gprof ./particle_simulator > profiling_report.txt

# Memory profiling with Valgrind
valgrind --tool=memcheck ./particle_simulator
```

## Contributing

1. Clone the repo
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Follow C++ coding standards (Google C++ Style Guide) - Learn C++ on the way there!
4. Add unit tests for new functionality
5. Update documentation as needed (Make sure there is documentation)
6. Commit your changes (`git commit -m 'Add some amazing feature'`)
7. Push to the branch (`git push origin feature/amazing-feature`)
8. Open a Pull Request

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contact

Team B - [team-b-email@example.com]

Project Link: [https://github.com/team-b/particle-simulator](https://github.com/team-b/particle-simulator)

## Acknowledgments

- OpenGL community and documentation
- C++ simulation programming resources
- Performance optimization guides and benchmarks
- Course instructors and teaching assistants

## Troubleshooting

### Common Issues

**OpenGL Context Creation Failed**
```bash
# Install Mesa drivers
sudo apt-get install mesa-utils libgl1-mesa-glx
```

**CMake Configuration Errors**
```bash
# Update CMake to latest version
sudo apt-get install cmake
```

**Docker Display Issues**
```bash
# Enable X11 forwarding
xhost +local:docker
```

For more troubleshooting, see [docs/troubleshooting.md](docs/troubleshooting.md).
