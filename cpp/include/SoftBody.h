#pragma once
#include <vector>
#include "Particle.h"
#include "Constraint.h"

namespace sim {
    class SoftBody {
    public:
        SoftBody(
            std::vector<Particle*> particles,
            std::vector<Constraint*> constraints = std::vector<Constraint*>(),
            double friction = 0.5,
            double restitution = 0.5
        );

        ~SoftBody();

        // Core function
        void applyForce(const Vector2& f);
        void solveConstraint();
        void update(double dt);

        // --- Accessors & mutators ----
        std::vector<Particle*> getParticles() { return particles; }
        std::vector<Constraint*> getConstraints() { return constraints; }
        double getFriction() { return friction; }
        double getRestitution() { return restitution; }

    protected:
        std::vector<Particle*> particles;
        std::vector<Constraint*> constraints;
        double friction;    // smooth 0 < 1 rough
        double restitution; // sticky 0 < 1 bounce
    };
}