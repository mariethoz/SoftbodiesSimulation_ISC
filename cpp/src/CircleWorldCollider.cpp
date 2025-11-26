#include "CircleWorldCollider.h"

InnerCircleCollider::InnerCircleCollider(Vector2 center, float radius)
    : center(center), radius(radius) {}

bool InnerCircleCollider::collide(Particle &p) {
    Vector2 toP = p.getPosition() - center;
    float dist = toP.length();
    float maxDist = radius - p.getRadius();

    if (dist > maxDist) {
        Vector2 n = toP / dist; // normalized
        p.setPosition(center + n * maxDist);
        return true;
    }
    return false;
}

OuterCircleCollider::OuterCircleCollider(Vector2 center, float radius)
    : center(center), radius(radius) {}

bool OuterCircleCollider::collide(Particle &p) {
    Vector2 toP = p.getPosition() - center;
    float dist = toP.length();
    float maxDist = radius + p.getRadius();

    if (dist < maxDist) {
        Vector2 n = toP / dist; // normalized
        p.setPosition(center + n * maxDist);
        return true;
    }
    return false;
}