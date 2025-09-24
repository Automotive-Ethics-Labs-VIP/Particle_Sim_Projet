
#include "ParticleSystem.h"

void ParticleSystem::addParticle(const Particle& particle) {
    particles.push_back(particle);
}

void ParticleSystem::update(float deltaTime) {
    for (auto& particle : particles) {
        particle.update(deltaTime);
    }
}

const std::vector<Particle>& ParticleSystem::getParticles() const {
    return particles;
}