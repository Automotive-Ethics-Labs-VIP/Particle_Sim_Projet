
#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp> // For vec2

class Particle {
public:
    
    Particle(glm::vec2 pos, float mass);     // Constructs a Particle at the given position with the specified mass.

    // Member Functions
    void applyForce(const glm::vec2& force); //A function to change the particle's acceleration based on a force (like gravity).
    void update(float deltaTime); // A function to update the particle's position based on its velocity over a small time step.

    // Member Variables
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec2 acceleration;
    float mass;
    float radius;
};

#endif // PARTICLE_H