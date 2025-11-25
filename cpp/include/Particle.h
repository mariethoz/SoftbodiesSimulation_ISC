#pragma once
#include "Vector2.h"

class Particle {
public:
    Particle(Vector2 pos, float mass = 1.0);

    void applyForce(const Vector2& f);
    void update(double dt);

private:
    Vector2 position;
    Vector2 prev_position;
    Vector2 forceAccum;
    float radius;
    float mass;
};
