#pragma once
#include "SoftBody.h"

class Simulation {
public:
    void addBody(SoftBody body);
    void step(double dt);

private:
    std::vector<SoftBody> bodies;

    //TODO add other solvers
    void applyGravity();
    void updateObjects(double dt);
};
