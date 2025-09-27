# Particle Simulator - Team B

A real-time particle simulation system built with C++ and OpenGL for physics simulation, performance optimization, and ML training data generation.

## 🚀 Quick Start

### Native Build (Recommended)

**macOS:**
```bash
# 1. Install dependencies
brew install cmake glfw glm

# 2. Build and run
cd /path/to/Particle_Sim_Projet
chmod +x build_and_run.sh
./build_and_run.sh

# 3. Or build manually
mkdir build_native && cd build_native
cmake ..
make -j4
./particle_simulator 100
```

**Linux (Ubuntu/Debian):**
```bash
# 1. Install dependencies
sudo apt-get update
sudo apt-get install build-essential cmake libglfw3-dev libglm-dev libgl1-mesa-dev

# 2. Build and run
cd /path/to/Particle_Sim_Projet
chmod +x build_and_run.sh
./build_and_run.sh

# 3. Or build manually
mkdir build_native && cd build_native
cmake ..
make -j$(nproc)
./particle_simulator 100
```

**Why Native is Better:**
- ✅ **Faster performance** - Direct hardware access
- ✅ **Better graphics** - Native OpenGL without virtualization
- ✅ **Easier debugging** - Direct terminal output and profiling
- ✅ **No display issues** - Works immediately on macOS/Linux

### Alternative: Docker
```bash
# Build image
docker build -t particle-simulator .

# Run simulation (requires X11 setup - see troubleshooting)
docker run --rm -e DISPLAY=host.docker.internal:0 particle-simulator ./particle_simulator 100
```

### Command Line Options
```bash
./particle_simulator 500          # Run with 500 particles
./particle_simulator --help       # Show help
./particle_simulator 1000         # Stress test with 1000 particles
```

### Expected Output
- **Graphics window** with colored particles moving and bouncing
- **Terminal output** showing FPS and performance metrics
- **JSON files** exported to `output/` directory for ML training

```
=== Particle Simulation System ===
[INIT] Initializing simulation systems...
[RENDER] OpenGL Version: 2.1 Metal - 88.1
[INIT] Created 100 particles
[RUN] Starting simulation main loop...
Particle Simulation - FPS: 60
```

**🎉 Success indicators:**
- Colored dots moving across the screen
- FPS counter showing ~60 FPS
- No error messages in terminal

---

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

### Docker Deployment (Optional)

**💡 Note:** Native builds are recommended for better performance and fewer setup issues. Use Docker for containerized deployments or CI/CD.

1. **Build Docker image**:
```bash
# Make script executable
chmod +x docker_manager.sh

# Build images
./docker_manager.sh build
```

2. **Run containerized simulation**:
```bash
# With display (Linux/macOS)
./docker_manager.sh run 500

# Headless mode (any platform)
./docker_manager.sh headless 1000

# Development environment
./docker_manager.sh dev
```

3. **Using Docker Compose**:
```bash
docker-compose up particle-simulator
```

## Usage

### Runtime Controls
```bash
# Basic usage
./particle_simulator [number_of_particles]

# Examples
./particle_simulator           # Default 500 particles
./particle_simulator 100       # Lightweight test
./particle_simulator 1000      # Performance test
./particle_simulator 2000      # Stress test
```

### Output Files
The simulation automatically generates:
- `output/simulation_data.json` - Real-time training data for ML
- `output/performance_profile.json` - Performance metrics and timing
- `output/final_simulation_data.json` - Complete dataset export

### Performance Monitoring
Watch the terminal output for:
- **FPS**: Current frames per second (target: 60+)
- **Physics Updates**: Physics calculations per second
- **Particle Count**: Number of active particles
- **Data Export Rate**: MB/hour of training data generated

## Project Structure

```
particle-simulator/
├── README.md
├── CMakeLists.txt
├── Dockerfile
├── .dockerignore
├── docker-compose.yml
├── docker_manager.sh
├── build_and_run.sh
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
│   ├── api_documentation.md
│   └── docker_guide.md
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

### Common Startup Issues

**❓ "Failed to initialize GLFW" or "Failed to create GLFW window"**
```bash
# macOS
brew install glfw

# Linux
sudo apt-get install libglfw3-dev libgl1-mesa-dev
```

**❓ "CMake Error" or build failures**
```bash
# Make sure CMake is recent enough
cmake --version  # Should be 3.10+

# Clean build
rm -rf build
mkdir build && cd build
cmake ..
make clean && make
```

**❓ Particles not visible or window is black**
- Make sure you're running in a directory with write permissions for `output/`
- Try a smaller particle count: `./particle_simulator 10`
- Check terminal output for error messages

**❓ "GLM not found" errors**
```bash
# macOS
brew install glm

# Linux
sudo apt-get install libglm-dev
```

**❓ Docker display issues (macOS/Linux)**
```bash
# macOS: Install and configure XQuartz
brew install --cask xquartz
open -a XQuartz
# In XQuartz preferences: Security → "Allow connections from network clients"
xhost +localhost

# Linux: Enable X11 forwarding
xhost +local:docker
```
**💡 Tip:** Most Docker display issues are avoided by using native builds instead.

**❓ Performance issues (low FPS)**
- Try fewer particles: `./particle_simulator 100`
- Close other graphics-intensive applications
- Check if GPU drivers are updated

### Getting Help
- Check the terminal output for specific error messages
- Run `./particle_simulator --help` for usage information
- Look in `docs/` directory for detailed documentation

---
