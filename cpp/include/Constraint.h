#pragma once
#include <nlohmann/json.hpp>

#include "Particle.h"
#include "Vector2.h"

using json = nlohmann::json;

namespace sim {
    /**
     * @class Constraint
     * @brief Represents a distance constraint between two particles.
     * 
     * Maintains a rest length between particles using
     * positional correction with stiffness and damping.
     */
    class Constraint {
    public:
        /**
         * @brief Construct a new Constraint object
         * @param part1 Pointer to the first particle
         * @param part2 Pointer to the second particle
         * @param stiffness Stiffness of the constraint (0 < stiffness <= 1)
         * @param damping Damping factor for oscillations (0 <= damping < 1)
         */
        Constraint(Particle* part1, Particle* part2, double stiffness = 0.8, double damping = 0.1);
        ~Constraint();
        
        // Core function
        /**
         * @brief Apply the constraint to the connected particles
         */
        void applyConstraint();

        // --- Accessors & mutators ----
        Vector2 getPart1() { return part1->getPosition(); }
        Vector2 getPart2() { return part2->getPosition(); }
        double getRestLength() { return restLength; }
        double getStiffness() { return stiffness; }
        double getDamping() { return damping; }

    private:
        Particle* part1;    /// Pointer to the first particle
        Particle* part2;    /// Pointer to the second particle
        double restLength;  /// Rest length between the two particles
        double stiffness;   /// Constraint stiffness [flexible 0 < 1 rigid]
        double damping;     /// Damping factor [oscilling 0 < 1 freezing]
    };
}