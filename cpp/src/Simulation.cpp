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

void Simulation::addCollider(WorldCollider *col) {
    colliders.push_back(col);
}

void Simulation::applyGravity() {
    for (auto& body : bodies)
        body.applyForce(gravity);
}

void Simulation::resolveCollisions() {
    collisionsWorld();
    collisionsBodies(); // body vs body collisions (particle-particle cross-body)
}

void Simulation::collisionsWorld() {
    for (auto& body : bodies) {
        for (auto& p : body.getParticles()) {
            for (auto collider : colliders) {
                collider->collide(p);
            }
        }
    }
    // const Vector2 position = {0,3};
    // const float radius = 8;
    // for(auto& b: bodies){
    //     for(auto& p: b.getParticles()){
    //         const Vector2 to_p = p.getPosition() - position;
    //         const float dist = to_p.length();
    //         const float maxDist = radius - p.getRadius();
    //         if (dist > maxDist) {
    //             const Vector2 n = to_p / dist; // normalized direction
    //             p.setPosition(position + n * maxDist);
    //         }
    //     }
    // }
}

void Simulation::collisionsBodies() {
    const int object_cnt = bodies.size();
    for (int i = 0; i < object_cnt; i++) {
        auto& obj1 = bodies[i];
        for (int j = i + 1; j < object_cnt; j++) {
            auto& obj2 = bodies[j];

            // Average friction coefficient
            float mu = 0.5f * (obj1.getFriction() + obj2.getFriction());
            // Average restitution (elasticity)
            float restitution = std::min(obj1.getRestitution(), obj2.getRestitution());

            for (auto& part1 : obj1.getParticles()) {
                for (auto& part2 : obj2.getParticles()) {

                    Vector2 delta = part1.getPosition() - part2.getPosition();
                    float dist = delta.length();
                    float min_dist = part1.getRadius() + part2.getRadius();
                    
                    if (dist > 0 && dist < min_dist) {
                        Vector2 n = delta / dist; // collision normal
                        float overlap = min_dist - dist;

                        // --- Penetration correction (positional) ---
                        float m1 = part1.getMass();
                        float m2 = part2.getMass();
                        float f1 = m1 / (m1 + m2);
                        float f2 = m2 / (m1 + m2);

                        part1.setPosition(part1.getPosition() + n * (overlap * f1));
                        part2.setPosition(part2.getPosition() - n * (overlap * f2));

                        // --- Velocity update (Verlet style) ---
                        Vector2 v1 = part1.getPosition() - part1.getPrevPosition();
                        Vector2 v2 = part2.getPosition() - part2.getPrevPosition();
                        Vector2 rel_v = v1 - v2;

                        // Normal impulse (restitution)
                        float vn = rel_v.dot(n);
                        float impulse = -(1.0f + restitution) * vn;
                        if (vn < 0) { // only resolve if moving toward each other
                            impulse /= (f1 + f2);

                            v1 += n * impulse * f1;
                            v2 -= n * impulse * f2;
                        }

                        // Tangent (friction)
                        Vector2 tangent = rel_v - n * rel_v.dot(n);
                        float tLen = tangent.length();
                        if (tLen > 1e-6f) {
                            tangent /= tLen;
                            float vt = rel_v.dot(tangent);

                            // Friction impulse
                            float maxFriction = mu * fabs(impulse);
                            float frictionImpulse = std::clamp(vt, -maxFriction, maxFriction);

                            v1 -= tangent * frictionImpulse * f1;
                            v2 += tangent * frictionImpulse * f2;
                        }

                        // --- Update prevPosition based on new velocities ---
                        part1.setPrevPosition(part1.getPosition() - v1);
                        part2.setPrevPosition(part2.getPosition() - v2);
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
