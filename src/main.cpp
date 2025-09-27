#include <iostream>
#include <random>
#include <chrono>
#include <thread>

// Core systems
#include "particle/ParticleSystem.h"
#include "physics/PhysicsEngine.h"
#include "rendering/Renderer.h"
#include "utils/JSONExporter.h"
#include "utils/PerformanceProfiler.h"

class ParticleSimulationApp {
private:
    // Core systems
    ParticleSystem m_particleSystem;
    PhysicsEngine m_physicsEngine;
    Renderer m_renderer;
    JSONExporter m_jsonExporter;
    PerformanceProfiler m_profiler;
    
    // Simulation parameters
    int m_particleCount;
    bool m_isRunning;
    float m_simulationTime;
    int m_frameCount;
    
    // Performance targets (from README)
    static const int TARGET_FPS = 60;
    static const int TARGET_PHYSICS_STEPS = 100;
    
    // Random generation
    std::random_device m_rd;
    std::mt19937 m_gen;
    
public:
    ParticleSimulationApp(int particleCount = 500) 
        : m_particleCount(particleCount)
        , m_isRunning(false)
        , m_simulationTime(0.0f)
        , m_frameCount(0)
        , m_gen(m_rd()) {
        
        // Configure systems
        m_profiler.setTargetFPS(TARGET_FPS);
        m_profiler.setTargetPhysicsSteps(TARGET_PHYSICS_STEPS);
        
        m_jsonExporter.setMaxFrames(500); // Limit memory usage
        m_jsonExporter.setExportOnDestroy(true);
        m_jsonExporter.setAutoExportFilename("output/simulation_data.json");
        
        std::cout << "=== Particle Simulation System ===" << std::endl;
        std::cout << "Target Performance:" << std::endl;
        std::cout << "  - 60+ FPS with real-time rendering" << std::endl;
        std::cout << "  - 100 physics steps per second" << std::endl;
        std::cout << "  - JSON data export capability" << std::endl;
        std::cout << "  - Performance profiling and optimization" << std::endl;
        std::cout << std::endl;
    }
    
    bool initialize() {
        std::cout << "[INIT] Initializing simulation systems..." << std::endl;
        
        // Initialize renderer
        if (!m_renderer.initialize(1280, 720, "Particle Simulation - Team B")) {
            std::cerr << "Failed to initialize renderer" << std::endl;
            return false;
        }
        
        // Set up viewport for particle world
        m_renderer.setViewport(glm::vec2(-100.0f, -100.0f), glm::vec2(100.0f, 100.0f));
        
        // Create initial particles
        createParticles();
        
        // Configure physics engine (disabled for now)
        m_physicsEngine.setGravity(glm::vec2(0.0f, 0.0f));    // No gravity
        m_physicsEngine.setAirResistance(0.0f);              // No air resistance
        m_physicsEngine.setCollisionDamping(0.8f);
        
        std::cout << "[INIT] Created " << m_particleCount << " particles" << std::endl;
        
        // Debug: Print first few particle positions
        const auto& particles = m_particleSystem.getParticles();
        std::cout << "[DEBUG] First 3 particle positions:" << std::endl;
        for (size_t i = 0; i < std::min(3, static_cast<int>(particles.size())); ++i) {
            std::cout << "  Particle " << i << ": world(" << particles[i].position.x 
                     << ", " << particles[i].position.y << ") radius: " << particles[i].radius;
            // Show screen coordinates too
            glm::vec2 screenPos = glm::vec2(particles[i].position.x / 100.0f, particles[i].position.y / 100.0f);
            std::cout << " screen(" << screenPos.x << ", " << screenPos.y << ")" << std::endl;
        }
        
        std::cout << "[INIT] Systems initialized successfully" << std::endl;
        
        return true;
    }
    
    void createParticles() {
        std::uniform_real_distribution<float> posDist(-100.0f, 100.0f); // Full coordinate range
        std::uniform_real_distribution<float> velDist(-5.0f, 5.0f);   // Smaller initial velocities
        std::uniform_real_distribution<float> massDist(0.5f, 2.0f);   // Reasonable mass
        std::uniform_real_distribution<float> radiusDist(1.0f, 3.0f); // Small radii
        
        for (int i = 0; i < m_particleCount; ++i) {
            glm::vec2 pos(posDist(m_gen), posDist(m_gen));
            float mass = massDist(m_gen);
            
            Particle particle(pos, mass);
            particle.velocity = glm::vec2(velDist(m_gen), velDist(m_gen));
            particle.radius = radiusDist(m_gen); // Use normal radius
            
            m_particleSystem.addParticle(particle);
        }
    }
    
    void run() {
        std::cout << "[RUN] Starting simulation main loop..." << std::endl;
        
        m_isRunning = true;
        const float deltaTime = 1.0f / TARGET_FPS;
        auto lastTime = std::chrono::high_resolution_clock::now();
        
        // Add some initial custom data for JSON export
        m_jsonExporter.addCustomData("simulation_type", "particle_physics");
        m_jsonExporter.addCustomData("target_fps", std::to_string(TARGET_FPS));
        m_jsonExporter.addCustomData("particle_count", std::to_string(m_particleCount));
        
        while (m_isRunning && !m_renderer.shouldClose()) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto frameDuration = std::chrono::duration<float>(currentTime - lastTime).count();
            lastTime = currentTime;
            
            // Begin profiling
            m_profiler.beginFrame();
            PROFILE_SCOPE(m_profiler, "total_frame");
            
            // Update simulation
            update(deltaTime);
            
            // Render
            render();
            
            // End profiling
            m_profiler.endFrame();
            m_profiler.updateFPS(m_renderer.getFPS());
            m_profiler.updateParticleCount(m_particleSystem.getParticles().size());
            
            // Export data periodically (every 30 frames for reasonable data rate)
            if (m_frameCount % 30 == 0) {
                m_jsonExporter.captureFrame(m_particleSystem, m_simulationTime, m_frameCount, m_renderer.getFPS());
            }
            
            // Performance reporting every 5 seconds
            if (m_frameCount % (TARGET_FPS * 5) == 0 && m_frameCount > 0) {
                printPerformanceReport();
            }
            
            m_frameCount++;
            m_simulationTime += deltaTime;
            
            // Exit after 30 seconds for demo
            if (m_simulationTime > 30.0f) {
                std::cout << "\n[DEMO] 30 second demo completed" << std::endl;
                break;
            }
        }
        
        cleanup();
    }
    
    void update(float deltaTime) {
        PROFILE_SCOPE(m_profiler, "physics_update");
        
        // Debug: Print update info
        static int updateCount = 0;
        if (updateCount % 60 == 0) {
            const auto& particles = m_particleSystem.getParticles();
            if (!particles.empty()) {
                std::cout << "[PHYSICS] Update " << updateCount << ": First particle pos(" 
                         << particles[0].position.x << ", " << particles[0].position.y 
                         << ") vel(" << particles[0].velocity.x << ", " << particles[0].velocity.y << ")" << std::endl;
            }
        }
        updateCount++;
        
        // Apply boundary constraints (full screen - prevent off-screen)
        m_physicsEngine.applyBoundaryConstraints(m_particleSystem, 
                                                glm::vec2(-100.0f, -100.0f), 
                                                glm::vec2(100.0f, 100.0f));
        
        // Add some interactive forces
        addInteractiveForces();
        
        // Update physics
        m_physicsEngine.integrateParticles(m_particleSystem, deltaTime);
    }
    
    void addInteractiveForces() {
        // No forces - just let particles move with initial velocity
    }
    
    void render() {
        PROFILE_SCOPE(m_profiler, "rendering");
        
        // Clear screen
        m_renderer.clear(glm::vec3(0.1f, 0.15f, 0.2f));
        
        // Render particles
        {
            PROFILE_SCOPE(m_profiler, "particle_rendering");
            m_renderer.renderParticleSystem(m_particleSystem);
        }
        
        // Present frame
        m_renderer.present();
        m_renderer.pollEvents();
    }
    
    void printPerformanceReport() {
        std::cout << "\n=== Performance Report (Frame " << m_frameCount << ") ===" << std::endl;
        std::cout << "Current FPS: " << m_renderer.getFPS() << std::endl;
        std::cout << "Average FPS: " << m_profiler.getAverageFPS() << std::endl;
        std::cout << "Target Met: " << (m_profiler.isTargetPerformanceMet() ? "YES" : "NO") << std::endl;
        std::cout << "Particles: " << m_particleSystem.getParticles().size() << std::endl;
        std::cout << "Frame Time: " << m_profiler.getFrameTime() << " ms" << std::endl;
        std::cout << "Data Export Rate: " << m_jsonExporter.getDataRate() << " MB/hour" << std::endl;
        
        // Show timing breakdown
        auto physicsData = m_profiler.getProfileData("physics_update");
        auto renderData = m_profiler.getProfileData("rendering");
        
        if (physicsData.callCount > 0) {
            std::cout << "Physics Update: " << physicsData.avgTime << " ms avg" << std::endl;
        }
        if (renderData.callCount > 0) {
            std::cout << "Rendering: " << renderData.avgTime << " ms avg" << std::endl;
        }
        std::cout << std::endl;
    }
    
    void cleanup() {
        std::cout << "\n[CLEANUP] Finalizing simulation..." << std::endl;
        
        // Export final performance data
        m_profiler.exportToFile("output/performance_profile.json");
        
        // Export final simulation data
        m_jsonExporter.exportToFile("output/final_simulation_data.json");
        
        // Print final report
        std::cout << "\n=== Final Performance Summary ===" << std::endl;
        std::cout << m_profiler.getPerformanceReport() << std::endl;
        
        std::cout << "=== Data Export Summary ===" << std::endl;
        std::cout << "Total frames captured: " << m_jsonExporter.getFrameCount() << std::endl;
        std::cout << "Data generation rate: " << m_jsonExporter.getDataRate() << " MB/hour" << std::endl;
        std::cout << "Total data size: " << (m_jsonExporter.getTotalDataSize() / 1024.0f) << " KB" << std::endl;
        
        std::cout << "\n[CLEANUP] Files exported to output/ directory:" << std::endl;
        std::cout << "  - simulation_data.json (continuous data)" << std::endl;
        std::cout << "  - final_simulation_data.json (complete dataset)" << std::endl;
        std::cout << "  - performance_profile.json (timing analysis)" << std::endl;
        
        m_isRunning = false;
    }
};

int main(int argc, char* argv[]) {
    int particleCount = 500;
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "--help" || arg == "-h") {
            std::cout << "Particle Simulation System - Team B" << std::endl;
            std::cout << "Usage: " << argv[0] << " [particle_count] [options]" << std::endl;
            std::cout << std::endl;
            std::cout << "Arguments:" << std::endl;
            std::cout << "  particle_count    Number of particles to simulate (1-2000, default: 500)" << std::endl;
            std::cout << std::endl;
            std::cout << "Options:" << std::endl;
            std::cout << "  --help, -h       Show this help message" << std::endl;
            std::cout << std::endl;
            std::cout << "Examples:" << std::endl;
            std::cout << "  " << argv[0] << "              # Run with 500 particles" << std::endl;
            std::cout << "  " << argv[0] << " 1000          # Run with 1000 particles" << std::endl;
            std::cout << "  " << argv[0] << " --help        # Show this help" << std::endl;
            std::cout << std::endl;
            std::cout << "Performance targets:" << std::endl;
            std::cout << "  - 60+ FPS with real-time rendering" << std::endl;
            std::cout << "  - JSON data export for ML training" << std::endl;
            std::cout << "  - Performance profiling and optimization" << std::endl;
            return 0;
        } else {
            // Try to parse as particle count
            try {
                particleCount = std::stoi(arg);
                particleCount = std::max(1, std::min(particleCount, 2000)); // Clamp range
            } catch (const std::exception&) {
                std::cerr << "Invalid argument: " << arg << std::endl;
                std::cerr << "Use --help for usage information" << std::endl;
                return 1;
            }
        }
    }
    
    std::cout << "Starting Particle Simulation with " << particleCount << " particles" << std::endl;
    std::cout << "Usage: " << argv[0] << " [particle_count]" << std::endl;
    std::cout << std::endl;
    
    try {
        ParticleSimulationApp app(particleCount);
        
        if (!app.initialize()) {
            std::cerr << "Failed to initialize simulation" << std::endl;
            return -1;
        }
        
        app.run();
        
        std::cout << "\nSimulation completed successfully!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
