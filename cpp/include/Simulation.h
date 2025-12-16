#pragma once
#include <vector>
#include <nlohmann/json.hpp>

#include "SoftBody.h"
#include "Vector2.h"
#include "WorldCollider.h"

using json = nlohmann::json;

namespace sim {
    /**
     * @brief Manages the physics simulation, including bodies and colliders.
     * 
     * Responsible for advancing the simulation using:
     * - Verlet integration
     * - Constraint solving
     * - Collision detection & response
     *
     * Simulation step order:
     * 1. Apply global forces (gravity)
     * 2. Solve constraints iteratively
     * 3. Resolve collisions
     * 4. Integrate particle positions
     */
    class Simulation {
    public:
        ~Simulation();
        
        /**
         * @brief Advances the simulation by a time step dt.
         * @param dt Time step to advance the simulation.
         */
        void step(double dt);

        /**
         * @brief Adds a soft body to the simulation.
         * @param body Pointer to the SoftBody to add.
         */
        void addBody(SoftBody* body);

        /**
         * @brief Adds a world collider to the simulation.
         * @param col Pointer to the WorldCollider to add.
         */
        void addCollider(WorldCollider* col);

        /**
         * @brief Clears all bodies and colliders from the simulation.
         */
        void clear();

        // --- Accessors & mutators ----
        std::vector<SoftBody*> getBodies() { return bodies; }
        std::vector<WorldCollider*> getColliders() { return colliders; }
        void setGravity(const Vector2 gravity) { this->gravity = gravity; }
        Vector2 getGravity() { return gravity; }

        // --- Saver & Loader ----
        json as_json();
        void from_json(json data);

    private:
        std::vector<SoftBody*> bodies;          /// Soft bodies in the simulation
        Vector2 gravity = Vector2();            /// Global gravity vector
        std::vector<WorldCollider*> colliders;  /// World colliders in the simulation

        // main steps
        void applyGravity();
        void updateObjects(double dt);
        void applyConstraints();
        void resolveCollisions(double dt);
        void collisionsWorld();
        void collisionsBodies(double dt);
    };
}