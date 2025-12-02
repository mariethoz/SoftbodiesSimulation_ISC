// main.cpp
#include <iostream>
#include <fstream>
#include "Simulation.h"
#include "PlaneWorldCollider.h"
#include "CircleWorldCollider.h"

using namespace sim;

// Helper to build a square soft body
SoftBody* createSquareBody(Vector2 center, double stiffness, double damping) {
    std::vector<Particle*> particles;
    double half = 1.0;

    // Define positions relative to center
    std::vector<Vector2> positions = {
        { center.x - half, center.y - half }, // bottom-left
        { center.x + half, center.y - half }, // bottom-right
        { center.x + half, center.y + half }, // top-right
        { center.x - half, center.y + half }  // top-left
    };

    for (auto& pos : positions) {
        particles.push_back(new Particle(pos, 1.0f));
    }

    std::vector<Constraint*> constraints;
    // Edges
    constraints.push_back(new Constraint(particles[0], particles[1], stiffness));
    constraints.push_back(new Constraint(particles[1], particles[2], stiffness));
    constraints.push_back(new Constraint(particles[2], particles[3], stiffness));
    constraints.push_back(new Constraint(particles[3], particles[0], stiffness));
    // Diagonals
    constraints.push_back(new Constraint(particles[0], particles[2], stiffness));
    constraints.push_back(new Constraint(particles[1], particles[3], stiffness));

    return new SoftBody(particles, constraints);
}

int main() {
    double step = 0.01;
    Simulation sim;
    sim.setGravity(Vector2(0,-10));
    sim.addCollider(new PlaneCollider(Vector2(0,1), -10.0f));
    sim.addCollider(new OuterCircleCollider( Vector2(0,-10), 5.0f ));
    sim.addCollider(new InnerCircleCollider( Vector2( 0,0), 15.0f ));

    // Add multiple squares with different stiffness/damping
    sim.addBody(createSquareBody(Vector2(-10, 0), 0.9, 0.1)); // stiff, low damping
    sim.addBody(createSquareBody(Vector2(0, 0), 0.5, 0.5));   // medium stiffness/damping
    sim.addBody(createSquareBody(Vector2(10, 0), 0.2, 0.8));  // soft, high damping

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