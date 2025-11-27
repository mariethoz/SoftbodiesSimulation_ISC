#pragma once
#include "SoftBody.h"
#include "Vector2.h"
#include "WorldCollider.h"
#include <vector>

namespace sim {
    class Simulation {
    public:
        void addBody(SoftBody body);
        void step(double dt);
        void addCollider(WorldCollider* col);

        // --- Accessors & mutators ----
        std::vector<SoftBody> getBodies() { return bodies; }
        void setGravity(const Vector2 gravity) { this->gravity = gravity; }

    private:
        std::vector<SoftBody> bodies;
        Vector2 gravity = Vector2();
        std::vector<WorldCollider*> colliders;

        // main steps
        void applyGravity();
        void resolveCollisions();
        void collisionsWorld();
        void collisionsBodies();
        void updateObjects(double dt);

    };
}