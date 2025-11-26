#pragma once
#include "WorldCollider.h"
#include "Vector2.h"

class PlaneCollider : public WorldCollider {
public:
    PlaneCollider(Vector2 normal, float d);
    
    bool collide(Particle& p) override;

private:
    Vector2 normal;
    float d;
};
