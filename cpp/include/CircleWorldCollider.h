#pragma once
#include <nlohmann/json.hpp>

#include "WorldCollider.h"
#include "Vector2.h"

using json = nlohmann::json;

namespace sim {
    /**
     * @brief Abstarct collider representing a circle in 2D space.
     * 
     * The circle can be either an inner or outer collider.
     */
    class CircleCollider : public WorldCollider {
        public:
            CircleCollider(Vector2 center, double radius, double friction = 0.1, double restitution = 0.9);
            ~CircleCollider();
            Vector2 getCenter() { return center; }
            double getRadius() { return radius; }
        protected:
            Vector2 center; /// Center position of the circle
            double radius;  /// Radius of the circle
    };

    /**
     * @brief Collider representing an inner circle.
     * 
     * Particles collide when they are inside the circle.
     */
    class InnerCircleCollider : public CircleCollider {
    public:
        InnerCircleCollider(Vector2 center, double radius, double friction = 0.1, double restitution = 0.9);
        
        bool collide(Particle* p, double friction, double restitution) override;

        json as_json() override;
    };

    /**
     * @brief Collider representing an outer circle.
     * 
     * Particles collide when they are outside the circle.
     */
    class OuterCircleCollider : public CircleCollider {
    public:
        OuterCircleCollider(Vector2 center, double radius, double friction = 0.1, double restitution = 0.9);
        
        bool collide(Particle* p, double friction, double restitution) override;

        json as_json() override;
    };
}