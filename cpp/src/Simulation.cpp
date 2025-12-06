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
    applyConstraints();

    // 3. Integrate particles (Verlet integration)
    updateObjects(dt);

    // 4. Resolve collisions (world boundaries, objects, etc.)
    resolveCollisions();
}

void Simulation::clear() {
    for (auto& b: bodies) {
        for (auto& p: b->getParticles()) {
            delete p;
        }
        b->getParticles().clear();
        for (auto& c: b->getConstraints()) {
            delete c;
        }
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

void Simulation::resolveCollisions() {
    collisionsWorld();
    collisionsBodies(); // body vs body collisions (particle-particle cross-body)
}

void Simulation::collisionsWorld() {
    for (auto& body : bodies) {
        for (auto& p : body->getParticles()) {
            for (auto collider : colliders) {
                collider->collide(p);
            }
        }
    }
}

void Simulation::collisionsBodies() {
    const int object_cnt = bodies.size();
    for (int i = 0; i < object_cnt; i++) {
        auto& obj1 = bodies[i];
        for (int j = i + 1; j < object_cnt; j++) {
            auto& obj2 = bodies[j];

            // Average friction coefficient
            double mu = 0.5f * (obj1->getFriction() + obj2->getFriction());
            // Average restitution (elasticity)
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

                        part1->setPosition(part1->getPosition() + n * (overlap * f1));
                        part2->setPosition(part2->getPosition() - n * (overlap * f2));
                    }
                }
            }
        }
    }
}
