#include "SoftBody.h"

using namespace sim;

SoftBody::SoftBody(std::vector<Particle*> particles)
    : SoftBody(particles, std::vector<Constraint*>(), 0.5, 0.5){}

SoftBody::SoftBody(std::vector<Particle*> particles, float friction, float restitution)
    : SoftBody(particles, std::vector<Constraint*>(), friction, restitution){}

SoftBody::SoftBody(std::vector<Particle*> part, std::vector<Constraint*> consts)
    : SoftBody(part, consts, 0.5, 0.5) {};

SoftBody::SoftBody(std::vector<Particle*> part , std::vector<Constraint*> consts, float f, float r)
    : particles(part), constraints(consts), friction(f), restitution(r) {}

SoftBody::~SoftBody() {
    std::cout << "SoftBody destroyed\n";
};

void SoftBody::applyForce(const Vector2 &f) {
    for (auto& p : particles) {
        p->applyForce(f);
    }
}

void SoftBody::solveConstaint() {
    for (auto& c: constraints) {
        c->applyConstraint();
    }
}

void SoftBody::update(double dt) {
    for (auto& p : particles) {
        p->update(dt);
    }
}