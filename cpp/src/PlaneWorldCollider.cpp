#include "PlaneWorldCollider.h"

PlaneCollider::PlaneCollider(Vector2 normal, float d)
    : normal(normal.normalized()), d(d) {};

bool PlaneCollider::collide(Particle &p) {
    float dist = p.getPosition().dot(normal) - d;

    if (dist > p.getRadius()) return false;

    float penetration = dist - p.getRadius();

    if (penetration < 0.0f) {
        p.setPosition(p.getPosition() - normal * penetration);
        return true;
    }
    return false;
}