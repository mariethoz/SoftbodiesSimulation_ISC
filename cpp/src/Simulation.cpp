#include "Simulation.h"
#include <iostream>
#include "Disc.h"

void Simulation::addBody(std::unique_ptr<Body> body) {
    bodies.push_back(std::move(body));
}

void Simulation::step(double dt) {
    for (auto& b : bodies) {
        b->update(dt);
    }

    // Collision detection (pairwise)
    for (size_t i = 0; i < bodies.size(); ++i) {
        for (size_t j = i+1; j < bodies.size(); ++j) {
            if (bodies[i]->intersects(*bodies[j])) {
                // For now just print collision
                // Later: collision response
                std::cout << "Collision detected!\n";
                resolveCollision(*bodies[i],*bodies[j]);
            }
        }
    }
}

void Simulation::resolveCollision(Body& a1, Body& b1) {
    Disc* a = dynamic_cast<Disc*>(&a1);
    if (!a) {
        throw std::runtime_error("Intersection not implemented for this shape");
    }
    Disc* b = dynamic_cast<Disc*>(&b1);
    if (!b) {
        throw std::runtime_error("Intersection not implemented for this shape");
    }
    Vector2 posA = a->getPosition();
    Vector2 posB = b->getPosition();

    Vector2 diff = posA - posB;
    double dist = std::sqrt(diff.dot(diff));
    double overlap = (a->getRadius() + b->getRadius()) - dist;

    if (overlap > 0) {
        // Normalize direction
        Vector2 normal = diff / dist;

        // Push discs apart equally (could weight by mass)
        double totalMass = a->getMass() + b->getMass();
        double moveA = overlap * (b->getMass() / totalMass);
        double moveB = overlap * (a->getMass() / totalMass);

        // Relative velocity
        Vector2 velA = a->getVelocity();
        Vector2 velB = b->getVelocity();
        Vector2 relVel = velA - velB;

        double velAlongNormal = relVel.x*normal.x + relVel.y*normal.y;
        if (velAlongNormal > 0) return; // already separating

        // Elastic collision impulse
        double restitution = 1.0; // perfectly elastic
        double impulseMag = -(1 + restitution) * velAlongNormal;
        impulseMag /= (1/a->getMass() + 1/b->getMass());

        Vector2 impulse = normal * impulseMag;
        Vector2 forceA = velA + impulse / a->getMass();
        Vector2 forceB = velB - impulse / b->getMass();
        a->setVelocity(forceA);
        b->setVelocity(forceB);
    }
}