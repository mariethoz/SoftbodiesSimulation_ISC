#pragma once
#include "Particle.h"

namespace sim {
    class WorldCollider {
    public:
        // Constructor with default values
        WorldCollider(double friction = 1.0, double restitution = 0.1)
            : worldFriction(friction), worldRestitution(restitution) {}

        virtual ~WorldCollider() = default;

        virtual bool collide(Particle* p, double friction, double restitution) = 0;

    protected:
        double worldFriction;    // smooth 0 < 1 rough
        double worldRestitution; // sticky 0 < 1 bounce
    };
}
