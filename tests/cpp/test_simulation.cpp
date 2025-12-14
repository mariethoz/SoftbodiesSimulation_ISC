#include <gtest/gtest.h>

#include "Simulation.h"
#include "PlaneWorldCollider.h"

using sim::Simulation;
using sim::SoftBody;
using sim::Particle;
using sim::Constraint;
using sim::Vector2;
using sim::PlaneCollider;

// --------------------------------------------------
// Helpers
// --------------------------------------------------

static SoftBody* makeSimpleBody(Vector2 pos, double radius = 1.0) {
    Particle* p = new Particle(pos, 1.0, radius);
    std::vector<Particle*> particles{ p };
    return new SoftBody(particles);
}

// --------------------------------------------------
// Construction & Basic Management
// --------------------------------------------------

TEST(SimulationTest, AddBodiesAndColliders) {
    Simulation sim;

    SoftBody* body = makeSimpleBody(Vector2(0, 0));
    PlaneCollider* plane = new PlaneCollider(Vector2(0, 1), 0.0);

    sim.addBody(body);
    sim.addCollider(plane);

    EXPECT_EQ(sim.getBodies().size(), 1);
    EXPECT_EQ(sim.getColliders().size(), 1);
}

TEST(SimulationTest, ClearDeletesEverything) {
    Simulation sim;

    sim.addBody(makeSimpleBody(Vector2(0, 0)));
    sim.addBody(makeSimpleBody(Vector2(5, 0)));
    sim.addCollider(new PlaneCollider(Vector2(0, 1), 0.0));

    EXPECT_NO_THROW(sim.clear());

    EXPECT_TRUE(sim.getBodies().empty());
    EXPECT_TRUE(sim.getColliders().empty());
}

// --------------------------------------------------
// Gravity
// --------------------------------------------------

TEST(SimulationTest, GravityMovesParticlesDownward) {
    Simulation sim;
    sim.setGravity(Vector2(0, -10));

    SoftBody* body = makeSimpleBody(Vector2(0, 0));
    sim.addBody(body);

    sim.step(1.0);

    auto p = body->getParticles()[0];
    EXPECT_LT(p->getPosition().y, 0.0);
}

// --------------------------------------------------
// Integration & Constraints
// --------------------------------------------------

TEST(SimulationTest, StepUpdatesParticlePositions) {
    Simulation sim;

    SoftBody* body = makeSimpleBody(Vector2(0, 0));
    sim.addBody(body);

    sim.step(1.0);

    auto p = body->getParticles()[0];
    EXPECT_EQ(p->getPosition(), Vector2(0, 0)); // no forces
}

TEST(SimulationTest, ConstraintsAreAppliedMultipleTimes) {
    Simulation sim;

    Particle* p1 = new Particle(Vector2(0, 0));
    Particle* p2 = new Particle(Vector2(10, 0));
    Constraint* c = new Constraint(p1, p2, 1.0, 0.0);

    SoftBody* body = new SoftBody({p1, p2}, {c});
    sim.addBody(body);

    // disturb
    p2->setPosition(Vector2(12, 0));
    p2->setPrevPosition(Vector2(12, 0));

    sim.step(1);
    sim.step(1);

    EXPECT_NEAR(p1->getPosition().x, 1.0, 1e-6);
    EXPECT_NEAR(p2->getPosition().x, 11.0, 1e-6);
}

// --------------------------------------------------
// World Collisions
// --------------------------------------------------

TEST(SimulationTest, WorldColliderIsInvoked) {
    Simulation sim;

    SoftBody* body = makeSimpleBody(Vector2(0, -1));
    sim.addBody(body);

    sim.addCollider(new PlaneCollider(Vector2(0, 1), 0.0));

    sim.step(1.0);

    auto p = body->getParticles()[0];
    EXPECT_GE(p->getPosition().y, 0.0);
}

// --------------------------------------------------
// Body-Body Collisions
// --------------------------------------------------

TEST(SimulationTest, BodiesCollideAndSeparate) {
    Simulation sim;

    Particle* p1 = new Particle(Vector2(0, 0), 1.0, 1.0);
    Particle* p2 = new Particle(Vector2(1.5, 0), 1.0, 1.0);

    // simulate approach
    p1->setPrevPosition(Vector2(-1, 0));
    p2->setPrevPosition(Vector2(2.5, 0));

    SoftBody* b1 = new SoftBody({p1});
    SoftBody* b2 = new SoftBody({p2});

    sim.addBody(b1);
    sim.addBody(b2);

    for (int i = 0; i < 100; i++) sim.step(0.01);

    EXPECT_GT((p1->getPosition() - p2->getPosition()).length(), 1.0);
}

// --------------------------------------------------
// Edge Cases
// --------------------------------------------------

TEST(SimulationTest, PinnedParticlesAreNotMovedByGravity) {
    Simulation sim;
    sim.setGravity(Vector2(0, -10));

    Particle* p = new Particle(Vector2(0, 10), 1.0, 1.0, true);
    SoftBody* body = new SoftBody({p});
    sim.addBody(body);

    sim.step(1.0);

    EXPECT_EQ(p->getPosition(), Vector2(0, 10));
}

TEST(SimulationTest, EmptySimulationDoesNotCrash) {
    Simulation sim;
    EXPECT_NO_THROW(sim.step(1.0));
}
