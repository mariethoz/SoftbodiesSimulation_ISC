#include "Constraint.h"

using namespace sim;


Constraint::Constraint(Particle *part1, Particle *part2, double stiffness, double damping)
    : part1(part1), part2(part2),
        restLength((part1->getPosition()-part2->getPosition()).length()),
        stiffness(stiffness), damping(damping) {}

Constraint::~Constraint() {}

void Constraint::applyConstraint() {
    Vector2 delta = part2->getPosition() - part1->getPosition();
    double dist = delta.length();
    if (dist < 1e-8) return;

    Vector2 dir = delta / dist;
    double diff = dist - restLength;

    // --- Positional correction (spring-like) ---
    Vector2 correction = dir * (stiffness * diff * 0.5);

    if (!part1->isPinned())
        part1->setPosition(part1->getPosition() + correction);
    if (!part2->isPinned())
        part2->setPosition(part2->getPosition() - correction);

    // --- Damping (optional, using prevPosition) ---
    Vector2 vel1 = (part1->getPosition() - part1->getPrevPosition());
    Vector2 vel2 = (part2->getPosition() - part2->getPrevPosition());
    Vector2 relVel = vel1 - vel2;

    double dampingForce = damping * relVel.dot(dir);
    Vector2 dampingImpulse = dir * dampingForce * 0.5;

    if (!part1->isPinned())
        part1->setPrevPosition(part1->getPrevPosition() + dampingImpulse);
    if (!part2->isPinned())
        part2->setPrevPosition(part2->getPrevPosition() - dampingImpulse);
}



