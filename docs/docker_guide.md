# Docker Containerization Guide

## Overview

The particle simulation system is fully containerized with Docker for easy deployment, consistent environments, and integration with other services (like Team A's ML systems).

## Quick Start

### 1. Build and Run (Automatic)
```bash
# Make scripts executable
chmod +x docker_manager.sh

# Build Docker images
./docker_manager.sh build

# Run with graphical display
./docker_manager.sh run 500

# Run headless (no display needed)
./docker_manager.sh headless 1000
```

### 2. Manual Docker Commands
```bash
# Build the image
docker build -t particle-sim:latest .

# Run with X11 forwarding (Linux/macOS)
docker run --rm -it \
  -e DISPLAY=$DISPLAY \
  -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
  -v $(pwd)/output:/app/output:rw \
  particle-sim:latest ./particle_simulator 500

# Run headless (any platform)
docker run --rm -it \
  -v $(pwd)/output:/app/output:rw \
  particle-sim:latest ./particle_simulator 1000
```

### 3. Using Docker Compose
```bash
# Run with display
docker-compose up particle-simulator

# Run headless for data generation  
docker-compose up particle-headless

# Development environment
docker-compose up particle-dev
```

## Container Images

### Production Image (`particle-sim:latest`)
- **Base**: Ubuntu 22.04
- **Size**: ~200MB (optimized runtime)
- **Includes**: OpenGL runtime, simulation binary, shaders
- **Use**: Production deployments, data generation

### Development Image (`particle-sim:dev`)
- **Base**: Ubuntu 22.04 with build tools
- **Size**: ~500MB (includes compiler, CMake)
- **Includes**: All build dependencies, source code
- **Use**: Development, debugging, compilation

## Platform Support

### Linux (Native)
✅ **Full OpenGL support** with X11 forwarding  
✅ **Hardware acceleration** available  
✅ **Best performance** and compatibility  

```bash
# Enable X11 forwarding for containers
xhost +local:docker

# Run with full GPU acceleration
docker run --rm -it \
  --gpus all \
  -e DISPLAY=$DISPLAY \
  -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
  particle-sim:latest ./particle_simulator 1000
```

### macOS
✅ **Software OpenGL** rendering supported  
⚠️ **X11 forwarding** requires XQuartz  
⚠️ **Performance** may be limited  

```bash
# Install XQuartz first
brew install --cask xquartz

# Allow connections from localhost
xhost +localhost

# Run with macOS display
docker run --rm -it \
  -e DISPLAY=host.docker.internal:0 \
  -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
  particle-sim:latest ./particle_simulator 500
```

### Windows
✅ **Software rendering** in WSL2  
⚠️ **Display forwarding** requires WSL2 + X server  
⚠️ **Best used headless** for data generation  

```bash
# Headless mode (recommended for Windows)
docker run --rm -it \
  -v "%cd%/output:/app/output" \
  particle-sim:latest ./particle_simulator 1000
```

## Data Persistence

### Output Volumes
The container exports data to mounted volumes:

```bash
# Local directories created automatically
./output/          # Simulation data (JSON)
./logs/           # Application logs
```

### Docker Volumes (Optional)
```bash
# Create named volumes for persistent storage
docker volume create particle-data
docker volume create particle-logs

# Run with named volumes
docker run --rm -it \
  -v particle-data:/app/output \
  -v particle-logs:/app/logs \
  particle-sim:latest ./particle_simulator 500
```

## Performance Optimization

### Resource Limits
```bash
# Limit CPU and memory usage
docker run --rm -it \
  --cpus="2.0" \
  --memory="2g" \
  particle-sim:latest ./particle_simulator 1000
```

### Multi-container Scaling
```yaml
# docker-compose.yml for parallel processing
version: '3.8'
services:
  sim1:
    image: particle-sim:latest
    command: ["./particle_simulator", "500"]
  sim2:
    image: particle-sim:latest
    command: ["./particle_simulator", "500"]
  sim3:
    image: particle-sim:latest
    command: ["./particle_simulator", "500"]
```

## Development Workflow

### 1. Development Container
```bash
# Start development environment
./docker_manager.sh dev

# Inside container: compile and test
cd /app/build
cmake .. && make -j$(nproc)
./particle_simulator 100
```

### 2. Hot Reload Development
```bash
# Mount source code for live editing
docker run --rm -it \
  -v $(pwd):/app:rw \
  -e DISPLAY=$DISPLAY \
  -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
  particle-sim:dev /bin/bash
```

### 3. Debugging
```bash
# Run with GDB for debugging
docker run --rm -it \
  --cap-add=SYS_PTRACE \
  -v $(pwd):/app:rw \
  particle-sim:dev gdb ./build/particle_simulator
```

## Integration with Team A

### API Endpoints (Future)
The container exposes port 8080 for REST API communication:

```bash
# Run with API port exposed
docker run --rm -it \
  -p 8080:8080 \
  particle-sim:latest ./particle_simulator --api-mode
```

### Data Exchange
```bash
# Shared volume for ML training data
docker run --rm -it \
  -v ml-training-data:/app/output \
  particle-sim:latest ./particle_simulator 2000

# Team A can access the same volume
docker run --rm -it \
  -v ml-training-data:/data \
  team-a-ml:latest python train_model.py --data /data
```

## Troubleshooting

### Display Issues
```bash
# Check X11 forwarding
echo $DISPLAY
xhost +local:docker

# Test OpenGL in container
docker run --rm -it \
  -e DISPLAY=$DISPLAY \
  -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
  particle-sim:latest glxinfo
```

### Permission Issues
```bash
# Fix file permissions
sudo chown -R $USER:$USER output/ logs/

# Run with specific user ID
docker run --rm -it \
  --user $(id -u):$(id -g) \
  particle-sim:latest ./particle_simulator 500
```

### Memory Issues
```bash
# Monitor container resource usage
docker stats particle-simulation

# Increase Docker memory limit in Docker Desktop
# Or use resource limits in docker-compose.yml
```

## Production Deployment

### Health Checks
```bash
# Container includes health check
docker run -d \
  --name particle-prod \
  --health-interval=30s \
  --health-timeout=10s \
  --health-retries=3 \
  particle-sim:latest ./particle_simulator 1000

# Check health status
docker ps
```

### Logging
```bash
# View container logs
docker logs particle-simulation

# Follow logs in real-time
docker logs -f particle-simulation

# Configure log rotation
docker run --rm -it \
  --log-driver json-file \
  --log-opt max-size=10m \
  --log-opt max-file=3 \
  particle-sim:latest ./particle_simulator 1000
```

### Security
```bash
# Run as non-root user (automatically configured)
docker run --rm -it \
  --user appuser \
  --read-only \
  --tmpfs /tmp \
  particle-sim:latest ./particle_simulator 500
```

## Cleanup

```bash
# Use the manager script
./docker_manager.sh cleanup

# Manual cleanup
docker stop $(docker ps -q --filter ancestor=particle-sim)
docker rmi particle-sim:latest particle-sim:dev
docker volume prune
docker system prune
```

This containerization setup provides a complete, portable environment for the particle simulation system that can be easily deployed, scaled, and integrated with other components of the Automotive Ethics Lab project.
