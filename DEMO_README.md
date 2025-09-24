# Team B Demo - Quick Start

## Running the Demo

1. **Make sure you have dependencies installed:**
   ```bash
   # Ubuntu/Debian
   sudo apt-get install libglfw3-dev libglm-dev libgl1-mesa-dev
   
   # macOS
   brew install glfw glm
   ```

2. **Run the demo:**
   ```bash
   chmod +x run_demo.sh
   ./run_demo.sh
   ```

## What the Demo Shows

- Basic particle physics simulation running at 60 FPS target
- Performance monitoring (actual vs target FPS)
- Foundation for Team B's simulation requirements:
  - Real-time physics updates
  - Performance metrics tracking
  - Data collection framework ready for ML integration

## Expected Output

```
=== Team B Demo - Particle Simulation ===
Running simulation for 5 seconds...
Target: 60 FPS, 100 physics steps/second
Frame 0 - Particle positions:
  Particle 0: (0, 0)
  Particle 1: (10, 0)
  Particle 2: (-5, 5)
...
=== Performance Results ===
Frames processed: 300
Actual FPS: ~60
Target met: YES
```

This demonstrates the foundation Team B needs for the automotive simulation environment.
