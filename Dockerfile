# Multi-stage Docker build for Particle Simulation System
# Team B - Automotive Ethics Lab

# Build stage
FROM ubuntu:22.04 AS builder

# Prevent interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install build dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    pkg-config \
    git \
    wget \
    # OpenGL and graphics dependencies
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    mesa-common-dev \
    # GLFW dependencies
    libglfw3-dev \
    # GLM (OpenGL Mathematics) 
    libglm-dev \
    # X11 libraries for display
    libx11-dev \
    libxrandr-dev \
    libxinerama-dev \
    libxcursor-dev \
    libxi-dev \
    # Additional OpenGL utilities
    mesa-utils \
    # Cleanup
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy source code
COPY src/ ./src/
COPY shaders/ ./shaders/
COPY docs/ ./docs/
COPY CMakeLists.txt .
COPY build_and_run.sh .
COPY *.md ./

# Create necessary directories
RUN mkdir -p build output logs

# Build the application
WORKDIR /app/build
RUN cmake .. && make -j$(nproc)

# Verify build was successful
RUN ls -la particle_simulator

# Runtime stage
FROM ubuntu:22.04 AS runtime

# Prevent interactive prompts
ENV DEBIAN_FRONTEND=noninteractive

# Install runtime dependencies only
RUN apt-get update && apt-get install -y \
    # OpenGL runtime libraries
    libgl1-mesa-glx \
    libglu1-mesa \
    # GLFW runtime
    libglfw3 \
    # X11 runtime libraries
    libx11-6 \
    libxrandr2 \
    libxinerama1 \
    libxcursor1 \
    libxi6 \
    # OpenGL utilities for testing
    mesa-utils \
    # System utilities
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# Create app user for security
RUN useradd -m -s /bin/bash appuser && \
    mkdir -p /app/output /app/logs && \
    chown -R appuser:appuser /app

# Set working directory
WORKDIR /app

# Copy built application from builder stage
COPY --from=builder /app/build/particle_simulator .
COPY --from=builder /app/shaders/ ./shaders/
COPY --from=builder /app/docs/ ./docs/
COPY --from=builder /app/*.md ./

# Set permissions
RUN chmod +x particle_simulator && \
    chown -R appuser:appuser /app

# Switch to non-root user
USER appuser

# Environment variables for OpenGL/X11
ENV DISPLAY=:0
ENV LIBGL_ALWAYS_INDIRECT=1

# Health check to verify the application exists and is executable
HEALTHCHECK --interval=30s --timeout=10s --start-period=5s --retries=3 \
    CMD test -x ./particle_simulator || exit 1

# Expose any potential API ports (for future Team A integration)
EXPOSE 8080

# Create volume mount points for output data
VOLUME ["/app/output", "/app/logs"]

# Default command with help information
CMD echo "=== Particle Simulation Container ===" && \
    echo "Available commands:" && \
    echo "  docker run --rm -it -e DISPLAY=\$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix particle-simulator ./particle_simulator [particles]" && \
    echo "  Example: docker run --rm -it -e DISPLAY=host.docker.internal:0 particle-simulator ./particle_simulator 500" && \
    echo "" && \
    echo "Starting simulation with 100 particles (safe for containers)..." && \
    ./particle_simulator 100

# Labels for container metadata
LABEL maintainer="Team B - Automotive Ethics Lab"
LABEL version="1.0"
LABEL description="Real-time particle simulation system with OpenGL rendering and ML data export"
LABEL org.opencontainers.image.source="https://github.com/team-b/particle-simulator"
