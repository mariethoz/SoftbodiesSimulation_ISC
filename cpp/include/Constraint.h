#pragma once
#include "Particle.h"
#include "Vector2.h"

namespace sim {
    class Constraint {
    public:
        Constraint(Particle* part1, Particle* part2);
        Constraint(Particle* part1, Particle* part2, float distance, float stiffness = 0.2);

        // Core function
        void applyConstraint();

        // --- Accessors & mutators ----
        Vector2 getPart1() { return part1->getPosition(); }
        Vector2 getPart2() { return part2->getPosition(); }
        float getRestLength() { return restLength; }

    private:
        Particle* part1;
        Particle* part2;
        float restLength;
        float stiffness;
    };
}