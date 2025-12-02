#pragma once
#include "WorldCollider.h"
#include "Vector2.h"

namespace sim {
    class CircleCollider : public WorldCollider {
        public:
            CircleCollider(Vector2 center, float radius);
            ~CircleCollider();
            Vector2 getCenter() { return center; }
            float getRadius() { return radius; }
        protected:
            Vector2 center;
            float radius;
    };

    class InnerCircleCollider : public CircleCollider {
    public:
        InnerCircleCollider(Vector2 center, float radius);
        
        bool collide(Particle* p) override;
    };

    class OuterCircleCollider : public CircleCollider {
    public:
        OuterCircleCollider(Vector2 center, float radius);
        
        bool collide(Particle* p) override;
    };
}