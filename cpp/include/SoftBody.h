#pragma once
#include <vector>
#include "Particle.h"

class SoftBody {
public:
    SoftBody(const std::vector<Particle>& particles);
    SoftBody(const std::vector<Particle>& particles, const float friction);

    // Core function
    void applyForce(const Vector2& f);
    void update(double dt);

    // --- Accessors & mutators ----
    std::vector<Particle>& getParticles() { return particles; }
    float getFriction() { return friction; }

protected:
    std::vector<Particle> particles;
    float friction; // smooth 0 < 1 rough
};