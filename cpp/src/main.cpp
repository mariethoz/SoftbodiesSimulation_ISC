// main.cpp
#include <iostream>
#include "utils.h"
#include "Simulation.h"
#include <fstream>

int main() {
    float step = 0.001;
    Simulation sim;
    sim.setGravity(Vector2(0,-10/step));

    // Define positions for each particle
    std::vector<Vector2> positions = {
        {  2.0f, -1.1f },
        {  2.0f,  1.1f },
        { -2.0f,  1.1f },
        {  0.0f, -2.0f },
        {  1.5f,  2.0f },
        { -3.0f,  0.5f },
        {  4.0f,  1.0f },
        { -1.5f, -1.5f },
        {  0.0f,  4.5f },
        {  2.5f, -3.0f }
    };


    // Create one SoftBody per particle
    for (auto& pos : positions) {
        std::vector<Particle> p;
        p.emplace_back(pos, 1.0f);   // single particle
        SoftBody body(p, 0.5f, 0.5f);
        sim.addBody(body);
    }
    
    std::ofstream out("visuals/positions.csv");
    for (SoftBody& b: sim.getBodies()) {
        for (Particle& p: b.getParticles()) {
            Vector2 pos = p.getPosition();
            out << "," << pos.x << "," << pos.y;
        }
    }
    out << "\n";

    for (int i = 1; i < 1000; i++) {
        sim.step(step);
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