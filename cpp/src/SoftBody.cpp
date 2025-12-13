#include "SoftBody.h"
#include <unordered_map>
#include <array>

using namespace sim;

SoftBody::SoftBody(
        std::vector<Particle *> particles,
        std::vector<Constraint *> constraints,
        double friction, double restitution
    )
    : border({}), particles(particles), constraints(constraints),
      friction(friction), restitution(restitution) {}

SoftBody::SoftBody(
        std::vector<Particle *> border,
        std::vector<Particle *> particles,
        std::vector<Constraint *> constraints,
        double friction, double restitution
    )
    : border(border), particles(particles), constraints(constraints),
      friction(friction), restitution(restitution) {}

SoftBody::~SoftBody() {
    std::cout << "SoftBody destroyed\n";
};

void SoftBody::applyForce(const Vector2 &f) {
    for (auto& p : particles) {
        p->applyForce(f);
    }
}

void SoftBody::solveConstraint() {
    for (auto& c : constraints) {
        c->applyConstraint();
    }
}


void SoftBody::update(double dt) {
    for (auto& p : particles) {
        p->update(dt);
    }
}