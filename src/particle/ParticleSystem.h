
#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "Particle.h"
#include <vector>

class ParticleSystem {
public:
    void addParticle(const Particle& particle);
    void update(float deltaTime);
    const std::vector<Particle>& getParticles() const;

private:
    std::vector<Particle> particles;
};

#endif // PARTICLE_SYSTEM_H