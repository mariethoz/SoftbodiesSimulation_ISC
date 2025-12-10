#include "Particle.h"
#include "Vector2.h"
#include <gtest/gtest.h>

using namespace sim;

// Helper: equality check for Vector2 with tolerance
static void expectVecNear(const Vector2& a, const Vector2& b, float tol = 1e-5f) {
    EXPECT_NEAR(a.x, b.x, tol);
    EXPECT_NEAR(a.y, b.y, tol);
}

TEST(ParticleTest, ConstructorInitializesCorrectly) {
    Vector2 pos(1.0f, 2.0f);
    Particle p(pos, 2.0f);

    expectVecNear(p.getPosition(), pos);
    EXPECT_FLOAT_EQ(p.getMass(), 2.0f);
    EXPECT_FLOAT_EQ(p.getInvMass(), 0.5f);
}

TEST(ParticleTest, DefaultMassIsOne) {
    Vector2 pos(0.0f, 0.0f);
    Particle p(pos);

    EXPECT_FLOAT_EQ(p.getMass(), 1.0f);
    EXPECT_FLOAT_EQ(p.getInvMass(), 1.0f);
}

TEST(ParticleTest, ImmovableParticleHasZeroInvMass) {
    Vector2 pos(0.0f, 0.0f);
    Particle p(pos, 0.0f);

    EXPECT_FLOAT_EQ(p.getMass(), 0.0f);
    EXPECT_FLOAT_EQ(p.getInvMass(), 0.0f);
}

TEST(ParticleTest, SettersUpdatePosition) {
    Vector2 pos(1.0f, 2.0f);
    Particle p(pos);

    Vector2 newPos(3.0f, 4.0f);
    p.setPosition(newPos);
    expectVecNear(p.getPosition(), newPos);
    expectVecNear(p.getPrevPosition(), pos);

    Vector2 prevPos(0.5f, 0.5f);
    p.setPrevPosition(prevPos);
    expectVecNear(p.getPrevPosition(), prevPos);
}

TEST(ParticleTest, ApplyForceAccumulates) {
    Vector2 pos(0.0f, 0.0f);
    Particle p(pos, 2.0f);

    p.applyForce(Vector2(2.0f, 0.0f));
    p.applyForce(Vector2(0.0f, 4.0f));

    // After two forces, acceleration should be (2,4)/mass = (1,2)
    p.update(1.0); // dt = 1 second

    // Position should have changed accordingly
    Vector2 newPos = p.getPosition();
    expectVecNear(Vector2(1.0f ,2.0f), p.getPosition());
    expectVecNear(pos, p.getPrevPosition());
}

TEST(ParticleTest, UpdateMovesParticle) {
    Vector2 pos(0.0f, 0.0f);
    Particle p(pos, 1.0f);

    p.applyForce(Vector2(1.0f, 0.0f)); // acceleration = (1,0)
    p.update(1.0); // dt = 1 second

    // Expect particle to move along +x
    expectVecNear(Vector2(1.0f ,0.0f), p.getPosition());
    expectVecNear(pos, p.getPrevPosition());
}

TEST(ParticleTest, CorrPositionAccumulatesAndResetsAfterUpdate) {
    Vector2 pos(0.0f, 0.0f);
    Particle p(pos, 1.0f);

    // Apply correction twice
    p.corrPosition(Vector2(1.0f, 0.0f));
    p.corrPosition(Vector2(0.0f, 2.0f));

    // Before update, correction should affect position
    p.update(1.0);

    // Expected position: (1,2)/2 because correction is applied
    expectVecNear(Vector2(1.0f, 2.0f), p.getPosition());

    // Correction should reset after update
    p.update(1.0);
    pos = p.getPosition();
    Vector2 prevPos = p.getPrevPosition();
    p.update(1.0);

    // No new forces or corrections, so position should equal prev_position
    expectVecNear(p.getPosition(), pos + (pos - prevPos));
}

TEST(ParticleTest, PinnedParticleFlagWorks) {
    Vector2 pos(0.0f, 0.0f);
    Vector2 gravity(0,-10);
    Particle p(pos, 1.0f, 1.0f, true);
    Particle q(pos, 1.0f, 1.0f, false);

    EXPECT_TRUE(p.isPinned());
    EXPECT_FALSE(q.isPinned());

    p.applyForce(gravity);
    q.applyForce(gravity);
    p.update(1.0);
    q.update(1.0);

    expectVecNear(p.getPosition(), p.getPrevPosition());
    expectVecNear(p.getPosition(), pos);
    expectVecNear(q.getPosition(), gravity);
}

TEST(ParticleTest, ForceResetAfterUpdate) {
    Vector2 pos(0.0f, 0.0f);
    Particle p(pos, 1.0f);

    p.applyForce(Vector2(5.0f, 0.0f));

    p.update(1.0);

    // ApplyForce should reset internally
    pos = p.getPosition();
    Vector2 prevPos = p.getPrevPosition();
    p.update(1.0);

    // No new forces or corrections, so particle should continue moving
    expectVecNear(p.getPrevPosition(), pos);
    expectVecNear(p.getPosition(), pos + (pos-prevPos));
}
