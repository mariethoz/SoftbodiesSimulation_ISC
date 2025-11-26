// main.cpp
#include <iostream>
#include "utils.h"
#include "Simulation.h"
#include <fstream>

int main() {
    Simulation sim;

    std::vector<Particle> p1;
    p1.emplace_back(Vector2(2, -1.1), 1.0f);
    std::vector<Particle> p2;
    p2.emplace_back(Vector2(2, 1.1), 1.0f);

    SoftBody body1 = SoftBody(p1, 0.0);
    sim.addBody(body1);
    SoftBody body2 = SoftBody(p2, 0.0);
    sim.addBody(body2);
    
    std::ofstream out("visuals/positions.csv");
    out << "0";
    for (SoftBody& b: sim.getBodies()) {
        for (Particle& p: b.getParticles()) {
            Vector2 pos = p.getPosition();
            out << "," << pos.x << "," << pos.y;
        }
    }
    out << "\n";

    for (int i = 1; i < 10000; i++) {
        sim.step(0.01);
        out << i;
        for (SoftBody& b: sim.getBodies()) {
            for (Particle& p: b.getParticles()) {
                Vector2 pos = p.getPosition();
                out << "," << pos.x << "," << pos.y;
            }
        }
        out << "\n";
    }

    out.close();

    std::cout << "Hello from MyProject!" << std::endl;
    std::cout << "2 + 3 = " << add(2, 3) << std::endl;
    return 0;
}