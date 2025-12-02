// main.cpp
#include <iostream>
#include <fstream>
#include "Simulation.h"
#include "PlaneWorldCollider.h"
#include "CircleWorldCollider.h"

using namespace sim;

int main() {
    float step = 0.01;
    Simulation sim;
    sim.setGravity(Vector2(0,-10));
    sim.addCollider(new PlaneCollider( Vector2(0,1), -10.0f ));
    sim.addCollider(new OuterCircleCollider( Vector2(0,-10), 5.0f ));
    sim.addCollider(new InnerCircleCollider( Vector2( 0,0), 15.0f ));

    
    // Define positions for each particle
    std::vector<Vector2> positions = {
        { -2.0f, -2.0f },  // bottom-left
        {  2.0f, -2.0f },  // bottom-right
        {  2.0f,  2.0f },  // top-right
        { -2.0f,  2.0f }   // top-left
    };


    // Create one SoftBody with 4 particles
    std::vector<Particle*> particles;
    for (auto& pos : positions) {
        particles.push_back(new Particle(pos, 1.0f));  // allocate particles
    }

    std::vector<Constraint*> constraints;
    float stiffness = 0.2;
    // Edge constraints
    constraints.push_back(new Constraint(particles[0], particles[1], stiffness));
    constraints.push_back(new Constraint(particles[1], particles[2], stiffness));
    constraints.push_back(new Constraint(particles[2], particles[3], stiffness));
    constraints.push_back(new Constraint(particles[3], particles[0], stiffness));
    // Diagonal constraints
    constraints.push_back(new Constraint(particles[0], particles[2], stiffness));
    constraints.push_back(new Constraint(particles[1], particles[3], stiffness));

    // Build the square body
    sim.addBody(new SoftBody(particles, constraints, 0.5f, 0.5f));
    
    std::ofstream out("visuals/positions.csv");
    out << 0;
    for (auto& b: sim.getBodies()) {
        for (auto& p: b->getParticles()) {
            Vector2 pos = p->getPosition();
            out << "," << pos.x << "," << pos.y;
        }
    }
    out << "\n";

    for (int i = 1; i < 1001; i++) {
        sim.step(step);
        out << i;
        for (auto& b: sim.getBodies()) {
            for (auto& p: b->getParticles()) {
                Vector2 pos = p->getPosition();
                out << "," << pos.x << "," << pos.y;
            }
        }
        out << "\n";
        if (i % 100 == 0) std::cout << "Step " << i << " completed\n";
    }

    out.close();

    std::cout << "Hello from MyProject!" << std::endl;
    std::cout << "It is the Constraints example for the animation." << std::endl;
    return 0;
}