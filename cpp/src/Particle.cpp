#include "Particle.h"

using namespace sim;

Particle::Particle(Vector2 pos, double m, double radius, bool p)
    : position(pos), mass(m), radius(radius), prev_position(pos), force_accum(0,0), pinned(p) {}

Particle::~Particle() {}

void Particle::applyForce(const Vector2& f) {
    force_accum += f;
}

void Particle::update(double dt) {
    if (pinned) return;
    Vector2 temp = position;
    Vector2 acceleration = force_accum / mass;

    // Apply Verlet integration with correction
    position += (position - prev_position) + acceleration * dt * dt;

    prev_position = temp;
    force_accum = Vector2(0,0);
}

json Particle::as_json()
{
    return position.as_json();
}

Vector2 Particle::from_json(json data)
{
    return Vector2::from_json(data);
}
