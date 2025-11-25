#include "Simulation.h"
#include <iostream>


void Simulation::addBody(SoftBody body) {
    bodies.push_back(std::move(body));
}

void Simulation::step(double dt) {
    // 1. Apply global forces (gravity)
    applyGravity();
    // 2. Integrate particles
    updateObjects(dt);
}

void Simulation::applyGravity() {
    Vector2 gravity(0, -10);
    for (auto& body : bodies)
        body.applyForce(gravity);
}

void Simulation::updateObjects(double dt) {
    for (auto& b : bodies) {
        b.update(dt);
    }
}