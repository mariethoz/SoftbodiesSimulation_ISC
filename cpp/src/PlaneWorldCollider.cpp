#include "PlaneWorldCollider.h"
#include <algorithm>

using namespace sim;

PlaneCollider::PlaneCollider(Vector2 normal, double d, double friction, double restitution)
    : WorldCollider(friction, restitution), normal(normal.normalized()), d(d){}

PlaneCollider::~PlaneCollider() {}

bool PlaneCollider::collide(Particle* p, double friction, double restitution) {
    if (p->isPinned()) return false;

    double dist = p->getPosition().dot(normal) - d;
    if (dist > p->getRadius()) return false;

    double penetration = p->getRadius() - dist;
    if (penetration > 0.0) {
        // --- Velocity (Verlet displacement) ---
        Vector2 vel = p->getPosition() - p->getPrevPosition();

        // --- Positional correction ---
        p->setPosition(p->getPosition() + normal * penetration);

        // --- Effective coefficients ---
        double effectiveFriction    = 0.5 * (worldFriction + friction);
        double effectiveRestitution = std::min(worldRestitution, restitution);

        // --- Decompose velocity ---
        double velAlongNormal = vel.dot(normal);
        Vector2 tangentVel = vel - velAlongNormal * normal;

        // --- Apply restitution on normal axis ---
        Vector2 correctedNormal = effectiveRestitution * velAlongNormal * normal;

        // --- Apply friction on tangent axis ---
        Vector2 correctedTangent = (1.0 - effectiveFriction) * tangentVel;

        // --- New corrected velocity ---
        Vector2 correctedVel = correctedNormal - correctedTangent;

        // --- Update prevPosition with corrected velocity ---
        p->setPrevPosition(p->getPosition() + correctedVel);

        return true;
    }
    return false;
}

json PlaneCollider::as_json()
{
    json data;
    data["ColliderType"] = COLLIDER_TYPE::PlaneColliderType;
    data["point"] = normal.as_json();
    data["distance"] = d;
    return data;
}
