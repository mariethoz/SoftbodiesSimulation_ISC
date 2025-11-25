// main.cpp
#include <iostream>
#include "utils.h"
#include "Simulation.h"

int main() {
    Simulation sim;

    std::vector<Particle> p;
    p.emplace_back(Vector2(0, 0), 1.0f);

    SoftBody body(p);
    sim.addBody(body);

    for (int i = 0; i < 10; i++) {
        std::cout << "Step " << i << "\n";
        sim.step(0.1);
    }

    std::cout << "Hello from MyProject!" << std::endl;
    std::cout << "2 + 3 = " << add(2, 3) << std::endl;
    return 0;
}