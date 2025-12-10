#pragma once
#include "WorldCollider.h"
#include "Vector2.h"

namespace sim {
    class PlaneCollider : public WorldCollider {
    public:
        PlaneCollider(Vector2 normal, double d, double friction = 0.9, double restitution = 0.1);
        ~PlaneCollider();

        bool collide(Particle* p, double friction, double restitution) override;
        Vector2 getNormal() { return normal; }
        double getDistance() { return d; }

    private:
        Vector2 normal;
        double d;
    };
}