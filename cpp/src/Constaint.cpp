#include "Constraint.h"

using namespace sim;

Constraint::Constraint(Particle *part1, Particle *part2)
    : Constraint(part1,part2,(part1->getPosition()-part2->getPosition()).length()){}

Constraint::Constraint(Particle *part1, Particle *part2, float restLength, float stiffness)
    : part1(part1), part2(part2), restLength(restLength), stiffness(stiffness) {}

void Constraint::applyConstraint(){
    Vector2 delta = part2->getPosition() - part1->getPosition();
    float dist = delta.length();
    if (dist == 0) return;

    float diff = (dist - restLength) / dist;
    Vector2 corr = delta * (0.5f * stiffness * diff);  // half correction

    if (!part1->isPinned()) part1->setPosition(part1->getPosition() + corr);
    if (!part2->isPinned()) part2->setPosition(part2->getPosition() - corr);

}