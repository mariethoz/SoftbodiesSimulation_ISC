#include "SoftBody.h"

SoftBody::SoftBody(const std::vector<Particle> &part)
    : particles(part) {};

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

void SoftBody::collision() {
}
