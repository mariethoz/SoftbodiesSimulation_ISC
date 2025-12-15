// main.cpp
#include <iostream>
#include <fstream>
#include "Simulation.h"
#include "PlaneWorldCollider.h"
#include "CircleWorldCollider.h"

using namespace sim;

// Helper to build a square soft body
SoftBody* createSquareBody(Vector2 center, double size,
            double u = 10,            // triangle edge length
            double mass = 1,
            double radius = 1,
            double stiffness = 0.8,
            double damping = 0.1,
            double friction = 0.5,
            double restitution = 0.5
        ) {
    std::vector<Particle*> particles;
    double half = size/2;

    // Define positions relative to center
    std::vector<Vector2> positions = {
        { center.x - half, center.y - half }, // bottom-left
        { center.x + half, center.y - half }, // bottom-right
        { center.x + half, center.y + half }, // top-right
        { center.x - half, center.y + half }  // top-left
    };

    return SoftBody::createFromPolygon(positions,u,mass,radius,stiffness,damping,friction,restitution);
}

// Helper to build a triangle soft body
SoftBody* createTriangleBody(Vector2 center, double size,
            double u = 10,            // triangle edge length
            double mass = 1,
            double radius = 1,
            double stiffness = 0.8,
            double damping = 0.1,
            double friction = 0.5,
            double restitution = 0.5
        ) {
    std::vector<Particle*> particles;
    double half = size/2;

    // Define positions relative to center
    std::vector<Vector2> positions = {
        { center.x - half, center.y - half }, // bottom-left
        { center.x + half, center.y - half }, // bottom-right
        { center.x       , center.y + half }, // top
    };

    return SoftBody::createFromPolygon(positions,u,mass,radius,stiffness,damping,friction,restitution);
}

int main() {
    double step = 0.01;
    Simulation sim;
    sim.setGravity(Vector2(0,-10));
    sim.addCollider(new PlaneCollider(Vector2(0,1), -100.0f));

    // Add multiple squares with different stiffness/damping
    sim.addBody(createSquareBody(Vector2(), 50));
    sim.addBody(createTriangleBody(Vector2(0,100), 50));

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
        std::cout << "\n";
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
    std::cout << "It is the body example for the animation." << std::endl;
    return 0;
}