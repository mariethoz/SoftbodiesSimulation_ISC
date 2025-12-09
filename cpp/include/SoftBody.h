#pragma once
#include <vector>
#include "Particle.h"
#include "Constraint.h"
#include "Vector2.h"

namespace sim {
    class SoftBody {
    public:
        static SoftBody* createFromPolygon(
            const std::vector<Vector2>& polygon,
            Vector2 center,
            int u = 10,            // triangle edge length
            double mass = 1,
            double radius = 1,
            double stiffness = 0.8,
            double damping = 0.1,
            double friction = 0.5,
            double restitution = 0.5
        );

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