#include "SoftBody.h"

SoftBody::SoftBody(const std::vector<Particle> &part)
    : particles(part), friction(0.5) {};

SoftBody::SoftBody(const std::vector<Particle> &part, const float f)
    : particles(part), friction(f) {};

void SoftBody::applyForce(const Vector2 &f) {
    for (auto& p : particles) {
        p.applyForce(f);
    }
}

void SoftBody::update(double dt) {
    for (auto& p : particles) {
        p.update(dt);
    }
}