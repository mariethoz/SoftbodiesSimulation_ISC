#include "Simulation.h"
#include <cmath>
#include <algorithm>
#include <iostream>

void Simulation::addBody(SoftBody body) {
    bodies.push_back(std::move(body));
}

void Simulation::step(double dt) {
    // 1. Apply global forces (gravity)
    applyGravity();

    // 2. Resolve Collision (broad & narrow)
    resolveCollisions();

    // 3. Integrate particles (Verlet)
    updateObjects(dt);
}

void Simulation::applyGravity() {
    Vector2 gravity(0.0f, -10.0f);
    for (auto& body : bodies)
        body.applyForce(gravity);
}

void Simulation::resolveCollisions() {
    collisionsWorld();
    collisionsBodies(); // body vs body collisions (particle-particle cross-body)
    collisionsWorld();  // optional second pass to stabilize (world after body-body)
}

void Simulation::collisionsWorld() {
    const Vector2 position = {0,3};
    const float radius = 8;
    for(auto& b: bodies){
        for(auto& p: b.getParticles()){
            const Vector2 to_p = p.getPosition() - position;
            const float dist = to_p.length();
            if (dist > radius - p.getRadius()) {
                const Vector2 n = to_p / dist;
                p.setPosition(position + n * (dist - p.getRadius()));
            }
        }
    }
}

Simulation::AABB Simulation::computeBodyAABB(SoftBody& b) const {
    auto& parts = b.getParticles();
    if (parts.empty()) return { Vector2(0,0), Vector2(0,0) };
    Vector2 min = parts[0].getPosition();
    Vector2 max = parts[0].getPosition();
    for (const auto& p : parts) {
        Vector2 pos = p.getPosition();
        float r = p.getRadius();
        min.x = std::min(min.x, pos.x - r);
        min.y = std::min(min.y, pos.y - r);
        max.x = std::max(max.x, pos.x + r);
        max.y = std::max(max.y, pos.y + r);
    }
    return { min, max };
}

void Simulation::collisionsBodies() {
    const int object_cnt = bodies.size();
    for (int i = 0; i<object_cnt; i++) {
        auto& obj1 = bodies[i];
        for (int j = i; j < object_cnt; j++) {
            auto& obj2 = bodies[j];
            for (auto& part1: obj1.getParticles()) {
                Vector2 pos1 = part1.getPosition();
                float r1 = part1.getRadius();
                for (auto& part2: obj2.getParticles()) {
                    const Vector2 coll = pos1 - part2.getPosition();
                    float dist = coll.length();
                    const float r = r1 + part2.getRadius();
                    if (dist > 0 && dist < r) {
                        Vector2 n = coll/dist;
                        float delta = r - dist;
                        Vector2 pos2 = part2.getPosition();
                        part1.setPosition(pos1 + n * 0.5 * delta);
                        part2.setPosition(pos2 - n * 0.5 * delta);
                    }
                }
            }
        }
    }
}

void Simulation::updateObjects(double dt) {
    for (auto& b : bodies) {
        b.update(dt);
    }
}
