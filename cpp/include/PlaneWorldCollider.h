#pragma once
#include <nlohmann/json.hpp>

#include "WorldCollider.h"
#include "Vector2.h"

using json = nlohmann::json;

namespace sim {
    class PlaneCollider : public WorldCollider {
    public:
        PlaneCollider(Vector2 normal, double d, double friction = 0.1, double restitution = 0.9);
        ~PlaneCollider();

        bool collide(Particle* p, double friction, double restitution) override;
        json as_json() override;
        
        Vector2 getNormal() { return normal; }
        double getDistance() { return d; }

    private:
        Vector2 normal;
        double d;
    };
}