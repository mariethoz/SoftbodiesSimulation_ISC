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
}

void Simulation::step(double dt)
{

    // 1. Apply global forces (gravity, wind, etc.)
    applyGravity();

    // 2. Satisfy constraints (distance constraints, springs, etc.)
    applyConstraints();

    // 3. Resolve collisions (world boundaries, objects, etc.)
    resolveCollisions(dt);

    // 4. Integrate particles (Verlet integration)
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
        b->getConstraints().clear();
        delete b;
    }
    bodies.clear();

    for (auto& c: colliders) {
        delete c;
    }
    colliders.clear();
}

json Simulation::as_json()
{
    json data;
    data["gravity"] = gravity.as_json();
    for (auto b: bodies)
        data["bodies"].push_back(b->as_json());
    for (auto c: colliders)
        data["colliders"].push_back(c->as_json());
    return data;
}

void Simulation::from_json(json data)
{
    this->clear();
    this->setGravity(Vector2::from_json(data["gravity"]));
    for (auto jb: data["bodies"])
        this->addBody(SoftBody::from_json(jb));
    for (auto jc: data["colliders"])
        this->addCollider(WorldCollider::from_json(jc));
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
    collisionsWorld();
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

/**
 * Axis-Aligned Bounding Box (AABB) structure
 */
struct AABB {
    Vector2 min;
    Vector2 max;
};

double INF = 1e300;

/**
 * @brief Computes the Axis-Aligned Bounding Box (AABB) for a set of particles.
 * 
 * @param particles The particles to compute the AABB for.
 * @return AABB The computed AABB.
 */
AABB computeAABB(std::vector<Particle*> particles) {
    AABB aabb;
    aabb.min = { INF,  INF};
    aabb.max = {-INF, -INF};

    for (auto& p : particles) {
        aabb.min.x = std::min(aabb.min.x, p->getPosition().x - p->getRadius());
        aabb.min.y = std::min(aabb.min.y, p->getPosition().y - p->getRadius());
        aabb.max.x = std::max(aabb.max.x, p->getPosition().x + p->getRadius());
        aabb.max.y = std::max(aabb.max.y, p->getPosition().y + p->getRadius());
    }
    return aabb;
}

/**
 * @brief Checks if two AABBs overlap.
 * 
 * @param a The first AABB.
 * @param b The second AABB.
 * @return true If the AABBs overlap.
 */
bool aabbOverlap(const AABB& a, const AABB& b) {
    return !(a.max.x < b.min.x || a.min.x > b.max.x ||
             a.max.y < b.min.y || a.min.y > b.max.y);
}

void Simulation::collisionsBodies(double dt) {
    const int object_cnt = bodies.size();
    for (int i = 0; i < object_cnt; i++) {
        auto& obj1 = bodies[i];
        AABB border1 = computeAABB(obj1->getParticles());
        for (int j = i + 1; j < object_cnt; j++) {
            auto& obj2 = bodies[j];

            AABB border2 = computeAABB(obj2->getParticles());
            if (!aabbOverlap(border1, border2)) continue;
            // Average friction coefficient
            double mu = 0.5 * (obj1->getFriction() + obj2->getFriction());
            // Minimum restitution
            double restitution = std::min(obj1->getRestitution(), obj2->getRestitution());

            for (auto& part1 : obj1->getParticles()) {
                for (auto& part2 : obj2->getParticles()) {

                    Vector2 delta = part1->getPosition() - part2->getPosition();
                    double dist = delta.length();
                    double min_dist = (part1->getRadius() + part2->getRadius());

                    if (dist > 0 && dist < min_dist) {
                        Vector2 n = delta / dist; // Collision normal
                        double overlap = min_dist - dist;

                        // --- Relative velocity ---
                        Vector2 relVel = (part1->getPosition() - part1->getPrevPosition()) 
                                    - (part2->getPosition() - part2->getPrevPosition());

                        double velAlongNormal = relVel.dot(n);
                        Vector2 tangentVel = relVel - velAlongNormal * n;

                        // --- Positional correction ---
                        double m1 = part1->getMass();
                        double m2 = part2->getMass();
                        double f1 = m1 / (m1 + m2);
                        double f2 = m2 / (m1 + m2);

                        if (!part1->isPinned())
                            part1->setPosition(part1->getPosition() + n * (overlap * f1));
                        if (!part2->isPinned())
                            part2->setPosition(part2->getPosition() - n * (overlap * f2));

                        // Only resolve if particles are moving toward each other
                        if (velAlongNormal < 0) {
                            double invMass1 = part1->isPinned() ? 0.0 : 1.0 / m1;
                            double invMass2 = part2->isPinned() ? 0.0 : 1.0 / m2;

                            // --- Apply restitution on normal axis ---
                            Vector2 correctedNormal = restitution * velAlongNormal * n;

                            // --- Apply friction on tangent axis ---
                            Vector2 correctedTangent = (1.0 - mu) * tangentVel;

                            // --- Combined correction ---
                            Vector2 correctedVel = correctedNormal + correctedTangent;

                            if (!part1->isPinned())
                                part1->setPrevPosition(part1->getPrevPosition() - correctedVel * invMass1 * dt);
                            if (!part2->isPinned())
                                part2->setPrevPosition(part2->getPrevPosition() + correctedVel * invMass2 * dt);
                        }
                    }
                }
            }
        }
    }
}
