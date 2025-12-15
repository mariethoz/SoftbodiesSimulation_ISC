#pragma once
#include <nlohmann/json.hpp>

#include "WorldCollider.h"
#include "Vector2.h"

using json = nlohmann::json;

namespace sim {
    class CircleCollider : public WorldCollider {
        public:
            CircleCollider(Vector2 center, double radius, double friction = 0.1, double restitution = 0.9);
            ~CircleCollider();
            Vector2 getCenter() { return center; }
            double getRadius() { return radius; }
        protected:
            Vector2 center;
            double radius;
    };

    class InnerCircleCollider : public CircleCollider {
    public:
        InnerCircleCollider(Vector2 center, double radius, double friction = 0.1, double restitution = 0.9);
        
        bool collide(Particle* p, double friction, double restitution) override;

        json as_json() override;
    };

    class OuterCircleCollider : public CircleCollider {
    public:
        OuterCircleCollider(Vector2 center, double radius, double friction = 0.1, double restitution = 0.9);
        
        bool collide(Particle* p, double friction, double restitution) override;

        json as_json() override;
    };
}