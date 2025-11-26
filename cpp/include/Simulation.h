#pragma once
#include "SoftBody.h"
#include "Vector2.h"
#include <vector>

class Simulation {
public:
    void addBody(SoftBody body);
    void step(double dt);

    // --- Accessors & mutators ----
    std::vector<SoftBody> getBodies() { return bodies; }
    void setWorldBounds(const Vector2& min, const Vector2& max) { worldMin = min; worldMax = max; }

private:
    std::vector<SoftBody> bodies;

    // world bounds
    Vector2 worldMin{ -10.0f, -10.0f };
    Vector2 worldMax{ 10.0f, 10.0f };

    // collision params
    float restitution = 0.5f;        // bounciness [0..1]
    float penetrationSlack = 0.01f;  // small slop to avoid jitter
    float correctionPercent = 0.8f;  // how much of penetration to correct

    // main steps
    void applyGravity();
    void resolveCollisions();
    void collisionsWorld();
    void collisionsBodies();
    void updateObjects(double dt);

    // helpers
    struct AABB { Vector2 min, max; };
    AABB computeBodyAABB(SoftBody& b) const;
};
