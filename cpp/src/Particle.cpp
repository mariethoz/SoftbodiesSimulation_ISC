#include "Particle.h"

using namespace sim;

Particle::Particle(Vector2 pos, double m, bool p)
    : position(pos), mass(m), radius(1.), prev_position(pos), force_accum(0,0), pinned(p) {
        corr_position = Vector2(0,0);
    }

Particle::~Particle() {
    std::cout << "Particle destroyed\n";
}

void Particle::applyForce(const Vector2& f) {
    force_accum += f;
}

void Particle::update(double dt) {
    Vector2 temp = position + corr_position/2;
    Vector2 acceleration = force_accum/mass;
    position += (corr_position + position - prev_position) + acceleration * dt * dt;
    prev_position = temp;
    corr_position = Vector2(0,0);
    force_accum = Vector2(0,0);
}