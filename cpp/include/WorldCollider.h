#pragma once
#include "Particle.h"

class WorldCollider {
public:
    virtual ~WorldCollider() = default;

    // Returns true if collision happened
    virtual bool collide(Particle& p) = 0;
};
