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
        Vector2 n = toP / dist; // collision normal

        // --- Positional correction ---
        p->setPosition(center + n * maxDist);

        // --- Effective coefficients ---
        double effectiveFriction    = 0.5 * (worldFriction + friction);
        double effectiveRestitution = std::min(worldRestitution, restitution);

        // --- Velocity response (Verlet displacement) ---
        Vector2 vel = p->getPosition() - p->getPrevPosition();
        double velAlongNormal = vel.dot(n);

        if (velAlongNormal < 0) {
            // Bounce with restitution
            Vector2 normalImpulse = -(1.0 + effectiveRestitution) * velAlongNormal * n;
            p->setPrevPosition(p->getPrevPosition() + normalImpulse);
        }

        // Friction along tangent
        Vector2 tangent = vel - velAlongNormal * n;
        if (tangent.length() > 1e-8) {
            tangent = tangent.normalized();
            double velAlongTangent = vel.dot(tangent);

            double frictionImpulseMag = std::clamp(
                velAlongTangent,
                -effectiveFriction * fabs(velAlongNormal),
                effectiveFriction * fabs(velAlongNormal)
            );
            Vector2 frictionImpulse = -frictionImpulseMag * tangent;

            p->setPrevPosition(p->getPrevPosition() + frictionImpulse);
        }

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
        Vector2 n = toP / dist; // collision normal

        // --- Positional correction ---
        p->setPosition(center + n * maxDist);

        // --- Effective coefficients ---
        double effectiveFriction    = 0.5 * (worldFriction + friction);
        double effectiveRestitution = std::min(worldRestitution, restitution);

        // --- Velocity response (Verlet displacement) ---
        Vector2 vel = p->getPosition() - p->getPrevPosition();
        double velAlongNormal = vel.dot(n);

        if (velAlongNormal < 0) {
            // Bounce with restitution
            Vector2 normalImpulse = -(1.0 + effectiveRestitution) * velAlongNormal * n;
            p->setPrevPosition(p->getPrevPosition() + normalImpulse);
        }

        // Friction along tangent
        Vector2 tangent = vel - velAlongNormal * n;
        if (tangent.length() > 1e-8) {
            tangent = tangent.normalized();
            double velAlongTangent = vel.dot(tangent);

            double frictionImpulseMag = std::clamp(
                velAlongTangent,
                -effectiveFriction * fabs(velAlongNormal),
                effectiveFriction * fabs(velAlongNormal)
            );
            Vector2 frictionImpulse = -frictionImpulseMag * tangent;

            p->setPrevPosition(p->getPrevPosition() + frictionImpulse);
        }

        return true;
    }
    return false;
}
