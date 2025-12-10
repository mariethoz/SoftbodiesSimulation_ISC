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

    PlaneCollider plane(Vector2(0.0f, 1.0f), 0.0f);
    sim.addCollider(&plane);

    EXPECT_EQ(sim.getBodies().size(), 1u);
    EXPECT_EQ(sim.getColliders().size(), 1u);
}

TEST(SimulationTest, ClearRemovesBodiesAndColliders) {
    Simulation sim;

    std::vector<Particle*> particles;
    particles.push_back(new Particle(Vector2(0.0f, 0.0f), 1.0f));
    sim.addBody(new SoftBody(particles));

    PlaneCollider plane(Vector2(0.0f, 1.0f), 0.0f);
    sim.addCollider(&plane);

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

    PlaneCollider plane(Vector2(0.0f, 1.0f), 0.0f); // y=0 plane
    sim.addCollider(&plane);

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

    InnerCircleCollider circle(Vector2(0.0f, 0.0f), 5.0f);
    sim.addCollider(&circle);

    sim.step(1.0);

    auto bs = sim.getBodies()[0];
    auto ps = bs->getParticles();
    // Expect particle corrected to inside circle
    double dist = std::sqrt(ps[0]->getPosition().x * ps[0]->getPosition().x +
                            ps[0]->getPosition().y * ps[0]->getPosition().y);
    EXPECT_LE(dist, 5.0);
}
