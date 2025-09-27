#include "PhysicsEngine.h"
#include <algorithm>
#include <cmath>

PhysicsEngine::PhysicsEngine() 
    : m_gravity(glm::vec2(0.0f, -9.81f))
    , m_airResistance(0.01f)
    , m_collisionDamping(0.8f) {
}

void PhysicsEngine::applyGravity(ParticleSystem& system, const glm::vec2& gravity) {
    // Access particles through the non-const method
    for (auto& particle : system.getParticles()) {
        glm::vec2 gravityForce = gravity * particle.mass;
        particle.applyForce(gravityForce);
    }
}

void PhysicsEngine::applyAirResistance(ParticleSystem& system, float resistance) {
    for (auto& particle : system.getParticles()) {
        // Air resistance opposes velocity: F = -k * v^2 * direction
        float speed = glm::length(particle.velocity);
        if (speed > 0.0f) {
            glm::vec2 dragDirection = -glm::normalize(particle.velocity);
            glm::vec2 dragForce = dragDirection * resistance * speed * speed;
            particle.applyForce(dragForce);
        }
    }
}

void PhysicsEngine::handleCollisions(ParticleSystem& system, float damping) {
    auto& particles = system.getParticles();
    
    for (size_t i = 0; i < particles.size(); ++i) {
        for (size_t j = i + 1; j < particles.size(); ++j) {
            if (checkCollision(particles[i], particles[j])) {
                resolveCollision(particles[i], particles[j], damping);
            }
        }
    }
}

void PhysicsEngine::integrateParticles(ParticleSystem& system, float deltaTime) {
    // Apply global forces first
    applyGravity(system, m_gravity);
    applyAirResistance(system, m_airResistance);
    
    // Handle collisions
    handleCollisions(system, m_collisionDamping);
    
    // Update particle physics
    system.update(deltaTime);
}

void PhysicsEngine::applyForceToParticle(Particle& particle, const glm::vec2& force) {
    particle.applyForce(force);
}

void PhysicsEngine::applyGlobalForce(ParticleSystem& system, const glm::vec2& force) {
    for (auto& particle : system.getParticles()) {
        particle.applyForce(force);
    }
}

void PhysicsEngine::applyBoundaryConstraints(ParticleSystem& system, const glm::vec2& minBounds, const glm::vec2& maxBounds) {
    for (auto& particle : system.getParticles()) {
        // Check X boundaries
        if (particle.position.x - particle.radius < minBounds.x) {
            particle.position.x = minBounds.x + particle.radius;
            particle.velocity.x = -particle.velocity.x * m_collisionDamping;
        }
        if (particle.position.x + particle.radius > maxBounds.x) {
            particle.position.x = maxBounds.x - particle.radius;
            particle.velocity.x = -particle.velocity.x * m_collisionDamping;
        }
        
        // Check Y boundaries
        if (particle.position.y - particle.radius < minBounds.y) {
            particle.position.y = minBounds.y + particle.radius;
            particle.velocity.y = -particle.velocity.y * m_collisionDamping;
        }
        if (particle.position.y + particle.radius > maxBounds.y) {
            particle.position.y = maxBounds.y - particle.radius;
            particle.velocity.y = -particle.velocity.y * m_collisionDamping;
        }
    }
}

bool PhysicsEngine::checkCollision(const Particle& p1, const Particle& p2) {
    float distance = calculateDistance(p1, p2);
    return distance < (p1.radius + p2.radius);
}

void PhysicsEngine::resolveCollision(Particle& p1, Particle& p2, float damping) {
    // Calculate collision normal
    glm::vec2 collisionNormal = p2.position - p1.position;
    float distance = glm::length(collisionNormal);
    
    if (distance == 0.0f) return; // Avoid division by zero
    
    collisionNormal = glm::normalize(collisionNormal);
    
    // Separate overlapping particles
    float overlap = (p1.radius + p2.radius) - distance;
    float separation = overlap * 0.5f;
    p1.position -= collisionNormal * separation;
    p2.position += collisionNormal * separation;
    
    // Calculate relative velocity
    glm::vec2 relativeVelocity = p2.velocity - p1.velocity;
    float velAlongNormal = glm::dot(relativeVelocity, collisionNormal);
    
    // Don't resolve if velocities are separating
    if (velAlongNormal > 0) return;
    
    // Calculate restitution (bounciness)
    float restitution = damping;
    
    // Calculate impulse scalar
    float j = -(1 + restitution) * velAlongNormal;
    j /= (1.0f / p1.mass) + (1.0f / p2.mass);
    
    // Apply impulse
    glm::vec2 impulse = j * collisionNormal;
    p1.velocity -= impulse / p1.mass;
    p2.velocity += impulse / p2.mass;
}

float PhysicsEngine::calculateDistance(const Particle& p1, const Particle& p2) {
    return glm::length(p2.position - p1.position);
}
