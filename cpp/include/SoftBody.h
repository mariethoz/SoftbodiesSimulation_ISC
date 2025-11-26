#pragma once
#include <vector>
#include "Particle.h"

class SoftBody {
public:
    SoftBody(const std::vector<Particle>& particles);
    SoftBody(const std::vector<Particle>& particles, const float friction, const float restitution);

    // Core function
    void applyForce(const Vector2& f);
    void update(double dt);

    // --- Accessors & mutators ----
    std::vector<Particle>& getParticles() { return particles; }
    float getFriction() { return friction; }
    float getRestitution() { return restitution; }

protected:
    std::vector<Particle> particles;
    float friction;    // smooth 0 < 1 rough
    float restitution; // sticky 0 < 1 bounce
};