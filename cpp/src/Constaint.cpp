#include "Constraint.h"

using namespace sim;


Constraint::Constraint(Particle *part1, Particle *part2, double stiffness, double damping)
    : part1(part1), part2(part2),
        restLength((part1->getPosition()-part2->getPosition()).length()),
        stiffness(stiffness), damping(damping) {}

Constraint::~Constraint() {
    std::cout << "Constraint destroyed\n";
}

void Constraint::applyConstraint() {
    Vector2 delta = part2->getPosition() - part1->getPosition();
    double dist = delta.length();
    if (dist < 1e-8) return;

    // --- positional correction ---
    double diff = (dist - restLength) / dist;
    Vector2 corr = delta * (0.5 * stiffness * diff);

    if (!part1->isPinned()) {
        part1->setPosition(part1->getPosition() + corr);
        part1->applyForce(corr * (1 - damping));
    }
    if (!part2->isPinned()) {
        part2->setPosition(part2->getPosition() - corr);
        part2->applyForce(corr * (1 - damping));
    }
}
