#include "Particle.h"

using namespace sim;

Particle::Particle(Vector2 pos, float m, bool p)
    : position(pos), mass(m), radius(1.), prev_position(pos), force_accum(0,0), pinned(p) {}

Particle::~Particle() {
    std::cout << "Particle destroyed\n";
}

void Particle::applyForce(const Vector2& f) {
    force_accum += f;
}

void Particle::update(double dt) {
    Vector2 temp = position;
    Vector2 acceleration = force_accum/mass;
    position += (position - prev_position) + acceleration * dt * dt;
    prev_position = temp;
    force_accum = {};
}