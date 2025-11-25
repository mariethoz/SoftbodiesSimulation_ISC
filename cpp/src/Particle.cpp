#include "Particle.h"
#include <iostream>

Particle::Particle(Vector2 pos, float m)
    : position(pos), mass(m), radius(1.), prev_position(pos), forceAccum(0,0) {}

void Particle::applyForce(const Vector2& f) {
    forceAccum += f;
}

void Particle::update(double dt) {
    Vector2 temp = position;
    Vector2 acceleration = forceAccum/mass;
    position += (position - prev_position) + acceleration * dt * dt;
    prev_position = temp;
    forceAccum = {};
    std::cout << position.y << "\n";
}
