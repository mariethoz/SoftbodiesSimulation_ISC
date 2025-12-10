#pragma once
#include "Particle.h"
#include "Vector2.h"

namespace sim {
    class Constraint {
    public:
        Constraint(Particle* part1, Particle* part2, double stiffness = 0.8, double damping = 0.1);
        ~Constraint();
        
        // Core function
        void applyConstraint();

        // --- Accessors & mutators ----
        Vector2 getPart1() { return part1->getPosition(); }
        Vector2 getPart2() { return part2->getPosition(); }
        double getRestLength() { return restLength; }

    private:
        Particle* part1;
        Particle* part2;
        double restLength;
        double stiffness;   // flexible 0 < 1 rigid
        double damping;     // oscilling 0 < 1 freezing
    };
}