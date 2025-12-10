#pragma once
#include "WorldCollider.h"
#include "Vector2.h"

namespace sim {
    class CircleCollider : public WorldCollider {
        public:
            CircleCollider(Vector2 center, double radius);
            ~CircleCollider();
            Vector2 getCenter() { return center; }
            double getRadius() { return radius; }
        protected:
            Vector2 center;
            double radius;
    };

    class InnerCircleCollider : public CircleCollider {
    public:
        InnerCircleCollider(Vector2 center, double radius);
        
        bool collide(Particle* p) override;
    };

    class OuterCircleCollider : public CircleCollider {
    public:
        OuterCircleCollider(Vector2 center, double radius);
        
        bool collide(Particle* p) override;
    };
}