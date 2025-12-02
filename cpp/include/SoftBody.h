#pragma once
#include <vector>
#include "Particle.h"
#include "Constraint.h"

namespace sim {
    class SoftBody {
    public:
        SoftBody(std::vector<Particle*> particles);
        SoftBody(std::vector<Particle*> particles, float friction, float restitution);
        SoftBody(std::vector<Particle*> particles, std::vector<Constraint*> constraints);
        SoftBody(std::vector<Particle*> particles, std::vector<Constraint*> constraints, float friction, float restitution);

        ~SoftBody();

        // Core function
        void applyForce(const Vector2& f);
        void solveConstaint();
        void update(double dt);

        // --- Accessors & mutators ----
        std::vector<Particle*> getParticles() { return particles; }
        std::vector<Constraint*> getConstraints() { return constraints; }
        float getFriction() { return friction; }
        float getRestitution() { return restitution; }

    protected:
        std::vector<Particle*> particles;
        std::vector<Constraint*> constraints;
        float friction;    // smooth 0 < 1 rough
        float restitution; // sticky 0 < 1 bounce
    };
}