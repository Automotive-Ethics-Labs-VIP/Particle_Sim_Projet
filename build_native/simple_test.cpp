#include <iostream>
#include "particle/ParticleSystem.h"

int main() {
    std::cout << "Testing particle creation..." << std::endl;
    
    ParticleSystem system;
    system.addParticle(Particle(glm::vec2(0, 0), 1.0f));
    system.addParticle(Particle(glm::vec2(10, 10), 2.0f));
    
    std::cout << "Created " << system.getParticles().size() << " particles" << std::endl;
    
    for (const auto& p : system.getParticles()) {
        std::cout << "Particle at (" << p.position.x << ", " << p.position.y << ")" << std::endl;
    }
    
    return 0;
}
