#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include "../particle/ParticleSystem.h"
#include <glm/glm.hpp>

class PhysicsEngine {
public:
    PhysicsEngine();
    
    // Core physics functions
    void applyGravity(ParticleSystem& system, const glm::vec2& gravity);
    void applyAirResistance(ParticleSystem& system, float resistance);
    void handleCollisions(ParticleSystem& system, float damping = 0.8f);
    void integrateParticles(ParticleSystem& system, float deltaTime);
    
    // Force application
    void applyForceToParticle(Particle& particle, const glm::vec2& force);
    void applyGlobalForce(ParticleSystem& system, const glm::vec2& force);
    
    // Boundary handling
    void applyBoundaryConstraints(ParticleSystem& system, const glm::vec2& minBounds, const glm::vec2& maxBounds);
    
    // Configuration
    void setGravity(const glm::vec2& gravity) { m_gravity = gravity; }
    void setAirResistance(float resistance) { m_airResistance = resistance; }
    void setCollisionDamping(float damping) { m_collisionDamping = damping; }
    
private:
    glm::vec2 m_gravity;
    float m_airResistance;
    float m_collisionDamping;
    
    // Helper functions
    bool checkCollision(const Particle& p1, const Particle& p2);
    void resolveCollision(Particle& p1, Particle& p2, float damping);
    float calculateDistance(const Particle& p1, const Particle& p2);
};

#endif // PHYSICS_ENGINE_H
