#include "Constraint.h"

using namespace sim;


Constraint::Constraint(Particle *part1, Particle *part2, float stiffness)
    : part1(part1), part2(part2),
        restLength((part1->getPosition()-part2->getPosition()).length()),
        stiffness(stiffness) {}

Constraint::~Constraint() {
    std::cout << "Constraint destroyed\n";
}

void Constraint::applyConstraint(){
    Vector2 delta = part2->getPosition() - part1->getPosition();
    float dist = delta.length();
    if (dist == 0) return;

    float diff = (dist - restLength) / dist;
    Vector2 corr = delta * (0.5f * stiffness * diff);  // half correction

    if (!part1->isPinned()) {
        part1->setPosition(part1->getPosition() + corr);
    }
    if (!part2->isPinned()) {
        part2->setPosition(part2->getPosition() - corr);
    }
}