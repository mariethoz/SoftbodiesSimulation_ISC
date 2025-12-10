#pragma once
#include "WorldCollider.h"
#include "Vector2.h"

namespace sim {
    class CircleCollider : public WorldCollider {
        public:
            CircleCollider(Vector2 center, double radius, double friction = 0.5, double restitution = 0.5);
            ~CircleCollider();
            Vector2 getCenter() { return center; }
            double getRadius() { return radius; }
        protected:
            Vector2 center;
            double radius;
    };

    class InnerCircleCollider : public CircleCollider {
    public:
        InnerCircleCollider(Vector2 center, double radius, double friction = 0.5, double restitution = 0.5);
        
        bool collide(Particle* p, double friction, double restitution) override;
    };

    class OuterCircleCollider : public CircleCollider {
    public:
        OuterCircleCollider(Vector2 center, double radius, double friction = 0.5, double restitution = 0.5);
        
        bool collide(Particle* p, double friction, double restitution) override;
    };
}