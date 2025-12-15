#pragma once
#include <vector>
#include <nlohmann/json.hpp>

#include "SoftBody.h"
#include "Vector2.h"
#include "WorldCollider.h"

using json = nlohmann::json;

namespace sim {
    class Simulation {
    public:
        ~Simulation();
        void step(double dt);
        void addBody(SoftBody* body);
        void addCollider(WorldCollider* col);

        // --- Accessors & mutators ----
        void clear();
        std::vector<SoftBody*> getBodies() { return bodies; }
        std::vector<WorldCollider*> getColliders() { return colliders; }
        void setGravity(const Vector2 gravity) { this->gravity = gravity; }
        Vector2 getGravity() { return gravity; }

        // --- Saver & Loader ----
        json as_json();
        Simulation* from_json(json data);

    private:
        std::vector<SoftBody*> bodies;
        Vector2 gravity = Vector2();
        std::vector<WorldCollider*> colliders;

        // main steps
        void applyGravity();
        void updateObjects(double dt);
        void applyConstraints();
        void resolveCollisions(double dt);
        void collisionsWorld();
        void collisionsBodies(double dt);

    };
}