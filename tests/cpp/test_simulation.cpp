#include "Simulation.h"
#include "SoftBody.h"
#include "Particle.h"
#include "PlaneWorldCollider.h"
#include "CircleWorldCollider.h"
#include "Vector2.h"
#include <gtest/gtest.h>

using namespace sim;

static void expectVecNear(const Vector2& a, const Vector2& b, float tol = 1e-5f) {
    EXPECT_NEAR(a.x, b.x, tol);
    EXPECT_NEAR(a.y, b.y, tol);
}

// ---------------- Simulation Tests ----------------

TEST(SimulationTest, AddBodyAndCollider) {
    Simulation sim;

    std::vector<Particle*> particles;
    particles.push_back(new Particle(Vector2(0.0f, 0.0f), 1.0f));
    sim.addBody(new SoftBody(particles));

    sim.addCollider(new PlaneCollider(Vector2(0.0f, 1.0f), 0.0f));

    EXPECT_EQ(sim.getBodies().size(), 1u);
    EXPECT_EQ(sim.getColliders().size(), 1u);
}

TEST(SimulationTest, ClearRemovesBodiesAndColliders) {
    Simulation sim;

    std::vector<Particle*> particles;
    particles.push_back(new Particle(Vector2(0.0f, 0.0f), 1.0f));
    sim.addBody(new SoftBody(particles));

    sim.addCollider(new PlaneCollider(Vector2(0.0f, 1.0f), 0.0f));

    sim.clear();

    EXPECT_TRUE(sim.getBodies().empty());
    EXPECT_TRUE(sim.getColliders().empty());
}

TEST(SimulationTest, GravityMovesParticlesDownward) {
    Simulation sim;
    sim.setGravity(Vector2(0.0f, -10.0f));

    std::vector<Particle*> particles;
    particles.push_back(new Particle(Vector2(0.0f, 0.0f), 1.0f));
    sim.addBody(new SoftBody(particles));

    sim.step(1.0); // one second

    auto bs = sim.getBodies()[0];
    auto ps = bs->getParticles();
    // Verlet: pos += (pos - prev) + a*dt^2
    // pos=0, prev=0, a=(0,-9.8), dt=1 → newPos=(0,-10)
    expectVecNear(ps[0]->getPosition(), Vector2(0.0f, -10.0f));
}

TEST(SimulationTest, CollisionsWithPlane) {
    Simulation sim;
    sim.setGravity(Vector2(0.0f, -10.0f));

    std::vector<Particle*> particles_1;
    particles_1.push_back(new Particle(Vector2(0.0f, -1.0f), 1.0f));
    sim.addBody(new SoftBody(particles_1));

    sim.addCollider(new PlaneCollider(Vector2(0.0f, 1.0f), 0.0f));

    sim.step(1.0);

    auto bs = sim.getBodies()[0];
    auto ps = bs->getParticles();
    // Expect collision resolution to keep particle at or above plane
    EXPECT_GE(ps[0]->getPosition().y, 0.0f);
}

TEST(SimulationTest, CollisionsWithInnerCircle) {
    Simulation sim;
    
    std::vector<Particle*> particles;
    particles.push_back(new Particle(Vector2(10.0f, 0.0f), 1.0f));
    sim.addBody(new SoftBody(particles));
    
    sim.addCollider(new InnerCircleCollider(Vector2(0.0f, 0.0f), 5.0f));

    sim.step(1.0);

    auto bs = sim.getBodies()[0];
    auto ps = bs->getParticles();
    // Expect particle corrected to inside circle
    double dist = std::sqrt(ps[0]->getPosition().x * ps[0]->getPosition().x +
                            ps[0]->getPosition().y * ps[0]->getPosition().y);
    EXPECT_LE(dist, 5.0);
}

TEST(SimulationTest, DestructorPrintsMessage) {
    testing::internal::CaptureStdout();
    {
        Simulation* sim = new Simulation();
        delete sim;
    }
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Simulation destroyed"), std::string::npos);
}

TEST(SimulationTest, CollisionsBetweenTwoBodies) {
    Simulation sim;

    // Body 1: particle at origin
    std::vector<Particle*> particles1;
    particles1.push_back(new Particle(Vector2(0.0f, 0.0f), 1.0f));
    sim.addBody(new SoftBody(particles1));

    // Body 2: particle overlapping with body 1
    std::vector<Particle*> particles2;
    particles2.push_back(new Particle(Vector2(0.5f, 0.0f), 1.0f));
    sim.addBody(new SoftBody(particles2));

    sim.step(1.0);

    auto ps1 = sim.getBodies()[0]->getParticles();
    auto ps2 = sim.getBodies()[1]->getParticles();

    // After collision resolution, particles should be separated
    double dist = (ps1[0]->getPosition() - ps2[0]->getPosition()).length();
    EXPECT_GE(dist, ps1[0]->getRadius() + ps2[0]->getRadius());
}

TEST(SimulationTest, CollisionsWithOuterCircle) {
    Simulation sim;

    std::vector<Particle*> particles;
    particles.push_back(new Particle(Vector2(0.0f, 0.0f), 1.0f));
    sim.addBody(new SoftBody(particles));

    sim.addCollider(new OuterCircleCollider(Vector2(0.0f, 0.0f), 5.0f));

    sim.step(1.0);

    auto ps = sim.getBodies()[0]->getParticles();
    double dist = ps[0]->getPosition().length();
    // Expect particle corrected to outside circle
    EXPECT_GE(dist, 5.0);
}

TEST(SimulationTest, ApplyConstraintsIsCalled) {
    Simulation sim;

    std::vector<Particle*> particles;
    particles.push_back(new Particle(Vector2(0.0f, 0.0f), 1.0f));
    particles.push_back(new Particle(Vector2(2.0f, 0.0f), 1.0f));

    // Add a simple constraint
    std::vector<Constraint*> constraints;
    constraints.push_back(new Constraint(particles[0], particles[1]));

    SoftBody* body = new SoftBody(particles, constraints);
    sim.addBody(body);

    sim.step(1.0);

    // After constraints, particles should be closer to rest length
    double dist = (particles[0]->getPosition() - particles[1]->getPosition()).length();
    EXPECT_NEAR(dist, body->getConstraints()[0]->getRestLength(), 1e-3);
}
