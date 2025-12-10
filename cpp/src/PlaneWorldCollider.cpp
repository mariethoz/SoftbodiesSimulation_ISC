#include "PlaneWorldCollider.h"
#include <algorithm>

using namespace sim;

PlaneCollider::PlaneCollider(Vector2 normal, double d, double friction, double restitution)
    : WorldCollider(friction, restitution), normal(normal.normalized()), d(d){}

PlaneCollider::~PlaneCollider() {
    std::cout << "PlaneCollider destroyed\n";
}

bool sim::PlaneCollider::collide(Particle* p, double friction, double restitution) {
    if (p->isPinned()) return false;

    double dist = p->getPosition().dot(normal) - d;

    // If particle is above plane, no collision
    if (dist > p->getRadius()) return false;

    // Penetration depth
    double penetration = p->getRadius() - dist;

    if (penetration > 0.0) {
        // --- Positional correction ---
        p->setPosition(p->getPosition() + normal * penetration);

        // --- Effective coefficients (world + local) ---
        double effectiveFriction    = 0.5 * (worldFriction + friction);
        double effectiveRestitution = std::min(worldRestitution, restitution);

        // --- Velocity response (Verlet displacement) ---
        Vector2 vel = p->getPosition() - p->getPrevPosition();

        // Normal component
        double velAlongNormal = vel.dot(normal);
        if (velAlongNormal < 0) {
            Vector2 normalImpulse = -(1.0 + effectiveRestitution) * velAlongNormal * normal;
            p->setPrevPosition(p->getPrevPosition() + normalImpulse);
        }

        // Tangent (friction)
        Vector2 tangent = vel - velAlongNormal * normal;
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