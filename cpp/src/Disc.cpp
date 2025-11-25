#include "Disc.h"
#include <stdexcept>

Disc::Disc(Particle part, double r)
    : center(part), radius(r) {}

void Disc::applyForce(Vector2& f) {
    center.applyForce(f);
}

void Disc::update(double dt) {
    center.update(dt);
}


bool Disc::intersects(const Body& other) const {
    const Disc* d = dynamic_cast<const Disc*>(&other);
    if (!d) {
        throw std::runtime_error("Intersection not implemented for this shape");
    }

    Vector2 posA = getPosition();
    Vector2 posB = d->getPosition();

    Vector2 diff = posA - posB;
    double distSq = diff.dot(diff);

    double radiusSum = radius + d->radius;
    return distSq <= radiusSum * radiusSum;
}