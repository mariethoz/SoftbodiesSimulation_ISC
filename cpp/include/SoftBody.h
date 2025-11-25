#pragma once
#include <vector>
#include "Particle.h"

class SoftBody {
public:
    SoftBody(const std::vector<Particle>& particles);

    // Access particles
    std::vector<Particle>& getParticles() { return particles; }

    // Core function
    void applyForce(const Vector2& f);
    void update(double dt);

protected:
    std::vector<Particle> particles;
};