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
            int mesh_unit = 10,
            double mass = 1,
            double radius = 1,
            double stiffness = 0.8,
            double damping = 0.1,
            double friction = 0.5,
            double restitution = 0.5
        );

        SoftBody(
            std::vector<Particle*> particles,
            std::vector<Constraint*> constraints = std::vector<Constraint*> {},
            double friction = 0.5, double restitution = 0.5
        );

        SoftBody(
            std::vector<Particle*> border,
            std::vector<Particle*> particles,
            std::vector<Constraint*> constraints,
            double friction = 0.5, double restitution = 0.5
        );

        ~SoftBody();

        // Core function
        void applyForce(const Vector2& f);
        void solveConstraint();
        void update(double dt);

        // --- Accessors & mutators ----
        std::vector<Particle*> getParticles() { return particles; }
        std::vector<Particle*> getBorder() { return border; }
        std::vector<Constraint*> getConstraints() { return constraints; }
        double getFriction() { return friction; }
        double getRestitution() { return restitution; }

    protected:
        std::vector<Particle*> particles;
        std::vector<Particle*> border;
        std::vector<Constraint*> constraints;
        double friction;    // smooth 0 < 1 rough
        double restitution; // sticky 0 < 1 bounce
    };
    

    // ---------------------------------------------------------------------------
    // Divide a segment into n equal subdivisions
    // ---------------------------------------------------------------------------
    static std::vector<Vector2> divideSegment(const Vector2& P1, const Vector2& P2, int n) {
        std::vector<Vector2> out;
        if (P1 == P2) {
            out.push_back(P1);
            return out;
        }
        if (n <= 0) {
            out.push_back(P1);
            out.push_back(P2);
            return out;
        }

        for (int i = 0; i <= n; ++i) {
            double t = double(i) / double(n);
            out.push_back(interpolate(P1, P2, t));
        }
        return out;
    }
}