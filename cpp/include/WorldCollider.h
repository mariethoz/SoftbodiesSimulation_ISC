#pragma once
#include <nlohmann/json.hpp>

#include "Particle.h"

using json = nlohmann::json;

namespace sim {
    class WorldCollider {
    public:
        // Constructor with default values
        WorldCollider(double friction = 1.0, double restitution = 0.1)
            : worldFriction(friction), worldRestitution(restitution) {}

        virtual ~WorldCollider() = default;

        virtual bool collide(Particle* p, double friction, double restitution) = 0;

        // --- Saver & Loader ----
        virtual json as_json() = 0;
        static WorldCollider* from_json(json data);

    protected:
        double worldFriction;    // smooth 0 < 1 rough
        double worldRestitution;
        // sticky 0 < 1 bounce
    };
    enum COLLIDER_TYPE {
        PlaneColliderType,
        OuterCircleColliderType,
        InnerCircleCollideTyper
    };
}
