#!/bin/bash

# Docker Build and Run Script for Particle Simulation
# Team B - Automotive Ethics Lab

set -e  # Exit on any error

# Configuration
IMAGE_NAME="particle-sim"
CONTAINER_NAME="particle-simulation"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Helper functions
log_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

log_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Function to check if Docker is installed and running
check_docker() {
    if ! command -v docker &> /dev/null; then
        log_error "Docker is not installed. Please install Docker first."
        exit 1
    fi

    if ! docker info &> /dev/null; then
        log_error "Docker daemon is not running. Please start Docker."
        exit 1
    fi

    log_success "Docker is available and running"
}

# Function to check if X11 forwarding is available
check_x11() {
    if [ -z "$DISPLAY" ]; then
        log_warning "DISPLAY environment variable is not set"
        log_warning "Graphical applications may not work"
        return 1
    fi

    if [ ! -d "/tmp/.X11-unix" ]; then
        log_warning "X11 socket directory not found"
        log_warning "Graphical applications may not work"
        return 1
    fi

    log_success "X11 display available at $DISPLAY"
    return 0
}

# Function to enable X11 access for containers
setup_x11() {
    log_info "Setting up X11 access for containers..."
    
    # Allow connections from local containers
    xhost +local:docker 2>/dev/null || {
        log_warning "Could not run xhost +local:docker"
        log_warning "You may need to run this manually for graphical display"
    }
}

# Function to build the Docker image
build_image() {
    log_info "Building Docker image: $IMAGE_NAME"
    
    # Build the main image
    docker build -t "$IMAGE_NAME:latest" . || {
        log_error "Failed to build Docker image"
        exit 1
    }
    
    # Build development image
    docker build --target builder -t "$IMAGE_NAME:dev" . || {
        log_error "Failed to build development Docker image"
        exit 1
    }
    
    log_success "Docker images built successfully"
    
    # Show image sizes
    log_info "Image sizes:"
    docker images | grep "$IMAGE_NAME"
}

# Function to run the simulation with display
run_with_display() {
    local particles=${1:-500}
    
    log_info "Running particle simulation with display ($particles particles)"
    
    # Ensure output directories exist
    mkdir -p output logs
    
    # Stop any existing container
    docker stop "$CONTAINER_NAME" 2>/dev/null || true
    docker rm "$CONTAINER_NAME" 2>/dev/null || true
    
    # Run with X11 forwarding
    docker run --rm -it \
        --name "$CONTAINER_NAME" \
        -e DISPLAY="$DISPLAY" \
        -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
        -v "$(pwd)/output:/app/output:rw" \
        -v "$(pwd)/logs:/app/logs:rw" \
        --user "$(id -u):$(id -g)" \
        "$IMAGE_NAME:latest" \
        ./particle_simulator "$particles"
}

# Function to run headless simulation
run_headless() {
    local particles=${1:-1000}
    
    log_info "Running headless particle simulation ($particles particles)"
    
    # Ensure output directories exist
    mkdir -p output logs
    
    # Run without display
    docker run --rm -it \
        --name "$CONTAINER_NAME-headless" \
        -e LIBGL_ALWAYS_SOFTWARE=1 \
        -v "$(pwd)/output:/app/output:rw" \
        -v "$(pwd)/logs:/app/logs:rw" \
        --user "$(id -u):$(id -g)" \
        "$IMAGE_NAME:latest" \
        ./particle_simulator "$particles"
}

# Function to run development container
run_dev() {
    log_info "Starting development container"
    
    # Stop any existing dev container
    docker stop "$CONTAINER_NAME-dev" 2>/dev/null || true
    docker rm "$CONTAINER_NAME-dev" 2>/dev/null || true
    
    # Run development container with source mounted
    docker run --rm -it \
        --name "$CONTAINER_NAME-dev" \
        -e DISPLAY="$DISPLAY" \
        -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
        -v "$(pwd):/app:rw" \
        --user "$(id -u):$(id -g)" \
        "$IMAGE_NAME:dev" \
        /bin/bash
}

# Function to use docker-compose
run_compose() {
    local service=${1:-particle-simulator}
    
    log_info "Running with docker-compose: $service"
    
    # Export DISPLAY for docker-compose
    export DISPLAY
    
    # Ensure output directories exist
    mkdir -p output logs
    
    # Run with docker-compose
    docker-compose up "$service"
}

# Function to clean up Docker resources
cleanup() {
    log_info "Cleaning up Docker resources"
    
    # Stop and remove containers
    docker stop "$CONTAINER_NAME" "$CONTAINER_NAME-dev" "$CONTAINER_NAME-headless" 2>/dev/null || true
    docker rm "$CONTAINER_NAME" "$CONTAINER_NAME-dev" "$CONTAINER_NAME-headless" 2>/dev/null || true
    
    # Remove images (optional)
    read -p "Remove Docker images? (y/N): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        docker rmi "$IMAGE_NAME:latest" "$IMAGE_NAME:dev" 2>/dev/null || true
        log_success "Docker images removed"
    fi
    
    # Reset X11 permissions
    xhost -local:docker 2>/dev/null || true
    
    log_success "Cleanup completed"
}

# Function to show usage
show_usage() {
    echo "Usage: $0 [COMMAND] [OPTIONS]"
    echo ""
    echo "Commands:"
    echo "  build                    Build Docker images"
    echo "  run [particles]          Run simulation with display (default: 500 particles)"
    echo "  headless [particles]     Run headless simulation (default: 1000 particles)"
    echo "  dev                      Start development container"
    echo "  compose [service]        Run with docker-compose"
    echo "  cleanup                  Clean up Docker resources"
    echo "  help                     Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0 build                 # Build the Docker images"
    echo "  $0 run 1000              # Run with 1000 particles"
    echo "  $0 headless 500          # Run headless with 500 particles"
    echo "  $0 dev                   # Start development environment"
    echo "  $0 compose particle-dev  # Run development service"
    echo ""
    echo "Docker images created:"
    echo "  $IMAGE_NAME:latest       # Production runtime image"
    echo "  $IMAGE_NAME:dev          # Development image with build tools"
}

# Main script logic
main() {
    echo "=== Particle Simulation Docker Manager ==="
    echo ""
    
    # Check Docker availability
    check_docker
    
    # Parse command line arguments
    case ${1:-help} in
        build)
            build_image
            ;;
        run)
            if check_x11; then
                setup_x11
                run_with_display "$2"
            else
                log_error "X11 display not available. Use 'headless' mode instead."
                exit 1
            fi
            ;;
        headless)
            run_headless "$2"
            ;;
        dev)
            if check_x11; then
                setup_x11
            fi
            run_dev
            ;;
        compose)
            run_compose "$2"
            ;;
        cleanup)
            cleanup
            ;;
        help|--help|-h)
            show_usage
            ;;
        *)
            log_error "Unknown command: $1"
            echo ""
            show_usage
            exit 1
            ;;
    esac
}

# Run main function with all arguments
main "$@"
