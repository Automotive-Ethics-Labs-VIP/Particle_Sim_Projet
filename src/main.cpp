#include <iostream>
#include <chrono>
#include <thread>
#include "particle/ParticleSystem.h"

int main() {
    std::cout << "=== Team B Demo - Particle Simulation ===" << std::endl;
    
    // Create particle system
    ParticleSystem system;
    
    // Add some test particles
    system.addParticle(Particle(glm::vec2(0.0f, 0.0f), 1.0f));
    system.addParticle(Particle(glm::vec2(10.0f, 0.0f), 2.0f));
    system.addParticle(Particle(glm::vec2(-5.0f, 5.0f), 1.5f));
    
    // Get particles for reading
    const auto& particles = system.getParticles();
    
    // Simulation loop - targeting 60 FPS as per Team B requirements
    const float targetFPS = 60.0f;
    const float deltaTime = 1.0f / targetFPS;
    
    auto startTime = std::chrono::high_resolution_clock::now();
    int frameCount = 0;
    
    std::cout << "Running simulation for 5 seconds..." << std::endl;
    std::cout << "Target: 60 FPS, 100 physics steps/second" << std::endl;
    
    while (frameCount < 300) { // 5 seconds at 60 FPS
        auto frameStart = std::chrono::high_resolution_clock::now();
        
        // Apply gravity force to all particles
        // Note: Need to modify ParticleSystem to allow force application
        // For now, just update without external forces
        
        // Update physics
        system.update(deltaTime);
        
        // Display progress every 60 frames (1 second)
        if (frameCount % 60 == 0) {
            std::cout << "Frame " << frameCount << " - Particle positions:" << std::endl;
            for (size_t i = 0; i < particles.size(); ++i) {
                const auto& p = particles[i];
                std::cout << "  Particle " << i << ": (" 
                         << p.position.x << ", " << p.position.y << ")" << std::endl;
            }
        }
        
        frameCount++;
        
        // Frame rate limiting
        auto frameEnd = std::chrono::high_resolution_clock::now();
        auto frameDuration = std::chrono::duration_cast<std::chrono::microseconds>(frameEnd - frameStart);
        auto targetFrameTime = std::chrono::microseconds(static_cast<long>(1000000 / targetFPS));
        
        if (frameDuration < targetFrameTime) {
            std::this_thread::sleep_for(targetFrameTime - frameDuration);
        }
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto totalTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    
    float actualFPS = frameCount / (totalTime.count() / 1000.0f);
    
    std::cout << "\n=== Performance Results ===" << std::endl;
    std::cout << "Frames processed: " << frameCount << std::endl;
    std::cout << "Total time: " << totalTime.count() << "ms" << std::endl;
    std::cout << "Actual FPS: " << actualFPS << std::endl;
    std::cout << "Target met: " << (actualFPS >= 55.0f ? "YES" : "NO") << std::endl;
    
    return 0;
}
