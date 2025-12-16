#pragma once
#include <nlohmann/json.hpp>

#include "Vector2.h"

using json = nlohmann::json;

namespace sim {
    /**
     * @brief A particle in 2D space with position, mass, radius, and pinned state.
     * 
     * The base element for physics simulations. Particles can have forces applied to them
     * and can be updated over time. They can also be pinned to remain stationary.
     */
    class Particle {
    public:
        Particle(Vector2 pos, double mass = 1.0, double radius = 1.0, bool pinned = false);
        ~Particle();

        // Core function
        /**
         * @brief Apply a force to the particle.
         * @param f The force vector to apply.
         */
        void applyForce(const Vector2& f);
        /**
         * @brief Update the particle's position based on accumulated forces and elapsed time.
         * @param dt The time step for the update.
         */
        void update(double dt);

        // --- Accessors & mutators ----
        const Vector2& getPosition() const { return position; }
        const Vector2& getPrevPosition() const { return prev_position; }
        void setPosition(const Vector2& p) { position = p; }
        void setPrevPosition(const Vector2& p) { prev_position = p; }

        double getRadius() const { return radius; }
        double getMass() const { return mass; }
        // inverse mass: 0 means immovable
        double getInvMass() const { return (mass <= 0.0f) ? 0.0f : (1.0f / mass); }

        bool isPinned() const { return pinned; }

        // --- Saver & Loader ----
        json as_json();
        static Vector2 from_json(json data);

    private:
        Vector2 position;       /// Current position of the particle
        Vector2 prev_position;  /// Previous position of the particle (for velocity calculation)
        Vector2 force_accum;    /// Accumulated forces acting on the particle
        double radius;          /// Radius of the particle
        double mass;            /// Mass of the particle
        bool pinned;            /// Whether the particle is pinned (immovable)
    };
}