#include "Particle.h"

Particle::Particle(Vector2 pos, double m)
    : position(pos), mass(m), velocity(0,0), forceAccum(0,0) {}

void Particle::applyForce(Vector2& f) {
    forceAccum += f;
}

void Particle::update(double dt) {
    // acceleration = F/m
    Vector2 acceleration = forceAccum / mass;
    velocity += acceleration * dt;
    position += velocity * dt;

    // reset forces
    forceAccum = {0,0};
}
