#include "Simulation.h"
#include <cmath>
#include <algorithm>
#include <iostream>

using namespace sim;

void Simulation::addBody(SoftBody* body) {
    bodies.push_back(body);
}

void Simulation::addCollider(WorldCollider* col) {
    colliders.push_back(col);
}

Simulation::~Simulation(){
    clear();
    std::cout << "Simulation destroyed\n";
}

void Simulation::step(double dt)
{

    // 1. Apply global forces (gravity, wind, etc.)
    applyGravity();

    // 2. Satisfy constraints (distance constraints, springs, etc.)
    for (int i = 0; i < 5; i++)
        applyConstraints();

    // 4. Resolve collisions (world boundaries, objects, etc.)
    resolveCollisions(dt);

    // 3. Integrate particles (Verlet integration)
    updateObjects(dt);
}

void Simulation::clear() {
    for (auto& b: bodies) {
        for (auto& c: b->getConstraints()) {
            delete c;
        }
        for (auto& p: b->getParticles()) {
            delete p;
        }
        b->getParticles().clear();
        delete b;
    }
    bodies.clear();

    for (auto& c: colliders) {
        delete c;
    }
    colliders.clear();
}


void Simulation::applyGravity() {
    for (auto& body : bodies)
        body->applyForce(gravity);
}

void Simulation::updateObjects(double dt) {
    for (auto& b : bodies) {
        b->update(dt);
    }
}

void Simulation::applyConstraints() {
    for (auto& b: bodies) {
        b->solveConstraint();
    }
}

void Simulation::resolveCollisions(double dt) {
    collisionsWorld();
    collisionsBodies(dt); // body vs body collisions (particle-particle cross-body)
}

void Simulation::collisionsWorld() {
    for (auto& body : bodies) {
        for (auto& p : body->getParticles()) {
            for (auto collider : colliders) {
                collider->collide(p, body->getFriction(), body->getRestitution());
            }
        }
    }
}

void Simulation::collisionsBodies(double dt) {
    const int object_cnt = bodies.size();
    for (int i = 0; i < object_cnt; i++) {
        auto& obj1 = bodies[i];
        for (int j = i + 1; j < object_cnt; j++) {
            auto& obj2 = bodies[j];

            // Average friction coefficient
            double mu = 0.5 * (obj1->getFriction() + obj2->getFriction());
            // Use minimum restitution (elasticity)
            double restitution = std::min(obj1->getRestitution(), obj2->getRestitution());

            for (auto& part1 : obj1->getParticles()) {
                for (auto& part2 : obj2->getParticles()) {

                    Vector2 delta = part1->getPosition() - part2->getPosition();
                    double dist = delta.length();
                    double min_dist = part1->getRadius() + part2->getRadius();

                    if (dist > 0 && dist < min_dist) {
                        Vector2 n = delta / dist; // collision normal
                        double overlap = min_dist - dist;

                        // --- Penetration correction (positional) ---
                        double m1 = part1->getMass();
                        double m2 = part2->getMass();
                        double f1 = m1 / (m1 + m2);
                        double f2 = m2 / (m1 + m2);

                        if (!part1->isPinned())
                            part1->setPosition(part1->getPosition() + n * (overlap * f1));
                        if (!part2->isPinned())
                            part2->setPosition(part2->getPosition() - n * (overlap * f2));
                        
                        // --- Velocity response (impulses) ---
                        Vector2 relVel = (part1->getPosition() - part1->getPrevPosition()) 
                                        - (part2->getPosition() - part2->getPrevPosition());
                        double velAlongNormal = relVel.dot(n);

                        // Only resolve if particles are moving toward each other
                        if (velAlongNormal < 0) {
                            double invMass1 = part1->isPinned() ? 0.0 : 1.0 / m1;
                            double invMass2 = part2->isPinned() ? 0.0 : 1.0 / m2;

                            // Normal impulse (restitution)
                            double j = -(1.0 + restitution) * velAlongNormal;
                            j /= invMass1 + invMass2;

                            Vector2 impulse = j * n;
                            if (!part1->isPinned())
                                part1->setPrevPosition(part1->getPrevPosition() - impulse * invMass1 * dt);
                            if (!part2->isPinned())
                                part2->setPrevPosition(part2->getPrevPosition() + impulse * invMass2 * dt);

                            // Friction impulse
                            Vector2 tangent = relVel - velAlongNormal * n;
                            if (tangent.length() > 1e-8)
                                tangent = tangent.normalized();

                            double jt = -relVel.dot(tangent);
                            jt /= invMass1 + invMass2;

                            double frictionImpulseMag = std::clamp(jt, -mu * j, mu * j);
                            Vector2 frictionImpulse = frictionImpulseMag * tangent;

                            if (!part1->isPinned())
                                part1->setPrevPosition(part1->getPrevPosition() - frictionImpulse * invMass1 * dt);
                            if (!part2->isPinned())
                                part2->setPrevPosition(part2->getPrevPosition() + frictionImpulse * invMass2 * dt);

                        }
                    }
                }
            }
        }
    }
}
