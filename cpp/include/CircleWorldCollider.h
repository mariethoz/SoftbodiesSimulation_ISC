#pragma once
#include "WorldCollider.h"
#include "Vector2.h"

class InnerCircleCollider : public WorldCollider {
public:
    InnerCircleCollider(Vector2 center, float radius);
    
    bool collide(Particle& p) override;

private:
    Vector2 center;
    float radius;
};

class OuterCircleCollider : public WorldCollider {
public:
    OuterCircleCollider(Vector2 center, float radius);
    
    bool collide(Particle& p) override;

private:
    Vector2 center;
    float radius;
};