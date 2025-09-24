
#include "Particle.h"

// Constructor implementation
Particle::Particle(glm::vec2 pos, float m) {
    position = pos;
    mass = m;
    velocity = glm::vec2(0.0f, 0.0f);
    acceleration = glm::vec2(0.0f, 0.0f);
    radius = 5.0f; // Default radius
}

void Particle::applyForce(const glm::vec2& force) {
    // Newton's second law: F = ma  =>  a = F/m
    acceleration += force / mass;
}

void Particle::update(float deltaTime) {
    velocity += acceleration * deltaTime;
    position += velocity * deltaTime;
    // Reset acceleration after applying it
    acceleration = glm::vec2(0.0f, 0.0f);
}