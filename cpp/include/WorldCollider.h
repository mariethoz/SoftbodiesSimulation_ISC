#pragma once
#include <nlohmann/json.hpp>

#include "Particle.h"

using json = nlohmann::json;

namespace sim {
    /**
     * @brief Abstract base class for world boundary colliders.
     * 
     * WorldColliders define static geometry used to collide
     * particles with the environment.
     */
    class WorldCollider {
    public:
        // Constructor with default values
        WorldCollider(double friction = 0.1, double restitution = 0.9)
            : worldFriction(friction), worldRestitution(restitution) {}

        virtual ~WorldCollider() = default;

        /**
         * @brief Handle collision of a particle with the collider
         * 
         * @param p Pointer to the particle to check for collision and respond to
         * @param friction Friction coefficient for the collision
         * @param restitution Restitution (bounciness) coefficient for the collision
         * @return true if a collision occurred and was handled, false otherwise
         */
        virtual bool collide(Particle* p, double friction, double restitution) = 0;

        // --- Saver & Loader ----
        virtual json as_json() = 0;
        static WorldCollider* from_json(json data);

    protected:
        double worldFriction;   /// Friction coefficient of the collider [smooth 0 < 1 rough]
        double worldRestitution; /// Restitution (bounciness) coefficient of the collider [sticky 0 < 1 reflect]
    };
    enum COLLIDER_TYPE {
        PlaneColliderType,
        OuterCircleColliderType,
        InnerCircleCollideTyper
    };
}
