#pragma once
#include <vector>
#include <memory>
#include "Body.h"

class Simulation {
public:
    void addBody(std::unique_ptr<Body> body);
    void resolveCollision(Body& a, Body& b);
    void step(double dt);

    const std::vector<std::unique_ptr<Body>>& getBodies() const { return bodies; }

private:
    std::vector<std::unique_ptr<Body>> bodies;
};
