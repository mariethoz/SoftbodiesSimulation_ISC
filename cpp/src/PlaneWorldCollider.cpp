#include "PlaneWorldCollider.h"

using namespace sim;

PlaneCollider::PlaneCollider(Vector2 normal, double d)
    : normal(normal.normalized()), d(d) {}

PlaneCollider::~PlaneCollider() {
    std::cout << "PlaneCollider destroyed\n";
};

bool PlaneCollider::collide(Particle* p) {
    double dist = p->getPosition().dot(normal) - d;

    if (dist > p->getRadius()) return false;

    double penetration = dist - p->getRadius();

    if (penetration < 0.0f) {
        p->setPosition(p->getPosition() - normal * penetration);
        return true;
    }
    return false;
}