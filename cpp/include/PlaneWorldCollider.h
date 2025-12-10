#pragma once
#include "WorldCollider.h"
#include "Vector2.h"

namespace sim {
    class PlaneCollider : public WorldCollider {
    public:
        PlaneCollider(Vector2 normal, double d);
        ~PlaneCollider();
        
        bool collide(Particle* p) override;
        Vector2 getNormal() { return normal; }
        double getDistance() { return d; }

    private:
        Vector2 normal;
        double d;
    };
}