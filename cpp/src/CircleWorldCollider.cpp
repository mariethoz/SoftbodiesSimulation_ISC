#include "CircleWorldCollider.h"
#include <algorithm>

using namespace sim;

CircleCollider::CircleCollider(Vector2 center, double radius, double friction, double restitution)
    : WorldCollider(friction, restitution), center(center), radius(radius) {}

CircleCollider::~CircleCollider() {
    std::cout << "CircleCollider destroyed\n";
}

InnerCircleCollider::InnerCircleCollider(Vector2 center, double radius, double friction, double restitution)
    : CircleCollider(center, radius, friction, restitution) {}

bool InnerCircleCollider::collide(Particle *p, double friction, double restitution) {
    if (p->isPinned()) return false;

    Vector2 toP = p->getPosition() - center;
    double dist = toP.length();
    double maxDist = radius - p->getRadius();

    if (dist == 0) {
        toP = Vector2(0, 1e-8);
        dist = 1e-8;
    }

    if (dist > maxDist) {
        Vector2 vel = p->getPosition() - p->getPrevPosition();
        Vector2 n = toP / dist; // Collision normal

        // --- Positional correction ---
        p->setPosition(center + n * maxDist);

        double effectiveFriction    = 0.5 * (worldFriction + friction);
        double effectiveRestitution = std::min(worldRestitution, restitution);

        double velAlongNormal = vel.dot(n);
        Vector2 tangentVel = vel - velAlongNormal * n;

        Vector2 correctedNormal = effectiveRestitution * velAlongNormal * n;
        Vector2 correctedTangent = (1.0 - effectiveFriction) * tangentVel;

        Vector2 correctedVel = correctedNormal + correctedTangent;
        p->setPrevPosition(p->getPosition() - correctedVel);

        return true;
    }
    return false;
}


OuterCircleCollider::OuterCircleCollider(Vector2 center, double radius, double friction, double restitution)
    : CircleCollider(center, radius, friction, restitution) {}

bool OuterCircleCollider::collide(Particle* p, double friction, double restitution) {
    if (p->isPinned()) return false;

    Vector2 toP = p->getPosition() - center;
    double dist = toP.length();
    double maxDist = radius + p->getRadius();

    if (dist == 0) {
        toP = Vector2(0, 1e-8);
        dist = 1e-8;
    }

    if (dist < maxDist) {
        Vector2 vel = p->getPosition() - p->getPrevPosition();
        Vector2 n = toP / dist; // Collision normal

        // --- Positional correction ---
        p->setPosition(center + n * maxDist);

        double effectiveFriction    = 0.5 * (worldFriction + friction);
        double effectiveRestitution = std::min(worldRestitution, restitution);

        double velAlongNormal = vel.dot(n);
        Vector2 tangentVel = vel - velAlongNormal * n;

        Vector2 correctedNormal = effectiveRestitution * velAlongNormal * n;
        Vector2 correctedTangent = (1.0 - effectiveFriction) * tangentVel;

        Vector2 correctedVel = correctedNormal + correctedTangent;
        p->setPrevPosition(p->getPosition() - correctedVel);

        return true;
    }
    return false;
}
