#pragma once
#include <nlohmann/json.hpp>

#include "WorldCollider.h"
#include "Vector2.h"

using json = nlohmann::json;

namespace sim {
    /**
     * @brief A collider representing an infinite plane in 2D space.
     * 
     * The plane is defined by a normal vector and a distance from the origin.
     */
    class PlaneCollider : public WorldCollider {
    public:
        /**
         * @brief Construct a new Plane Collider object
         * 
         * @param normal The normal vector of the plane (should be normalized)
         * @param d The distance from the origin along the normal
         * @param friction Friction coefficient for collisions with this plane
         * @param restitution Restitution (bounciness) coefficient for collisions with this plane
         */
        PlaneCollider(Vector2 normal, double d, double friction = 0.1, double restitution = 0.9);
        ~PlaneCollider();

        bool collide(Particle* p, double friction, double restitution) override;
        json as_json() override;
        
        Vector2 getNormal() { return normal; }
        double getDistance() { return d; }

    private:
        Vector2 normal;     /// Normal vector of the plane
        double d;           /// Distance from the origin along the normal
    };
}