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
    constraints.push_back(new Constraint(particles[0], particles[1], stiffness, damping));
    constraints.push_back(new Constraint(particles[1], particles[2], stiffness, damping));
    constraints.push_back(new Constraint(particles[2], particles[3], stiffness, damping));
    constraints.push_back(new Constraint(particles[3], particles[0], stiffness, damping));
    // Diagonals
    constraints.push_back(new Constraint(particles[0], particles[2], stiffness, damping));
    constraints.push_back(new Constraint(particles[1], particles[3], stiffness, damping));

    return new SoftBody(particles, constraints);
}

// Helper to build a triangle soft body
SoftBody* createTriangleBody(Vector2 center, double stiffness, double damping) {
    std::vector<Particle*> particles;
    double half = 1.0;

    // Define positions relative to center
    std::vector<Vector2> positions = {
        { center.x - half, center.y - half }, // bottom-left
        { center.x + half, center.y - half }, // bottom-right
        { center.x       , center.y + half }, // top
    };

    for (auto& pos : positions) {
        particles.push_back(new Particle(pos, 1.0f));
    }

    std::vector<Constraint*> constraints;
    // Edges
    constraints.push_back(new Constraint(particles[0], particles[1], stiffness, damping));
    constraints.push_back(new Constraint(particles[0], particles[2], stiffness, damping));
    constraints.push_back(new Constraint(particles[1], particles[2], stiffness, damping));

    return new SoftBody(particles, constraints);
}

int main() {
    double step = 0.01;
    Simulation sim;
    sim.setGravity(Vector2(0,-10));
    sim.addCollider(new PlaneCollider(Vector2(0,1), -10.0f));
    // sim.addCollider(new OuterCircleCollider( Vector2(0,-10), 5.0f ));
    // sim.addCollider(new InnerCircleCollider( Vector2( 0,0), 15.0f ));

    // Add multiple squares with different stiffness/damping
    sim.addBody(createSquareBody(Vector2(30, 10), 0.9, 0.1)); // very rigid, springs back quickly (snappy, little energy loss)
    sim.addBody(createSquareBody(Vector2(25, 10), 0.5, 0.5)); // balanced: moderate rigidity and moderate damping (steady, natural motion)
    sim.addBody(createSquareBody(Vector2(20, 10), 0.2, 0.8)); // floppy but heavily damped (soft, sluggish, resists oscillation)

    std::ofstream out("visuals/positions.csv");
    out << 0;
    for (auto& b: sim.getBodies()) {
        for (auto& p: b->getParticles()) {
            Vector2 pos = p->getPosition();
            out << "," << pos.x << "," << pos.y;
        }
    }
    out << "\n";

    for (int i = 1; i < 2001; i++) {
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