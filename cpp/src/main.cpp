// main.cpp
#include <iostream>
#include "utils.h"
#include "Simulation.h"
#include "Disc.h"

int main() {
    Simulation sim;

    auto d1 = std::make_unique<Disc>(Particle({0,0}, 1.0), 1.0);
    auto d2 = std::make_unique<Disc>(Particle({2.5,0}, 1.0), 1.0);

    d1->applyForce(Vector2(10,0)); // push to the right
    sim.addBody(std::move(d1));
    sim.addBody(std::move(d2));

    for (int i = 0; i < 10; ++i) {
        sim.step(0.1);
        
        // Assuming Simulation has a way to access bodies:
        const auto& bodies = sim.getBodies();
        std::cout << "Step " << i << ":\n";
        for (const auto& b : bodies) {
            auto pos = b->getPosition();
            std::cout << pos.x << ", " << pos.y << "\n";
        }
    }
    std::cout << "Hello from MyProject!" << std::endl;
    std::cout << "2 + 3 = " << add(2, 3) << std::endl;
    return 0;
}