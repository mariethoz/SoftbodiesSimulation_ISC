#pragma once
#include "Vector2.h"

class Particle {
public:
    Particle(Vector2 pos, double mass = 1.0);

    void applyForce(Vector2& f);
    void update(double dt);

    Vector2 getPosition() const { return position; }
    Vector2 getVelocity() const {return velocity; }
    void setVelocity(Vector2 v) {velocity = v; }
    double getMass() const {return mass; }


private:
    Vector2 position;
    Vector2 velocity;
    Vector2 forceAccum;
    double mass;
};
