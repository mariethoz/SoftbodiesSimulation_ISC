#include <gtest/gtest.h>

#include "Particle.h"

using sim::Particle;
using sim::Vector2;

// --------------------------------------------------
// Construction & Basic Properties
// --------------------------------------------------

TEST(ParticleTest, ConstructorInitializesCorrectly) {
    Vector2 pos(1.0, 2.0);
    Particle p(pos, 2.0, 0.5, false);

    EXPECT_EQ(p.getPosition(), pos);
    EXPECT_EQ(p.getPrevPosition(), pos);
    EXPECT_DOUBLE_EQ(p.getMass(), 2.0);
    EXPECT_DOUBLE_EQ(p.getRadius(), 0.5);
    EXPECT_FALSE(p.isPinned());
}

TEST(ParticleTest, DefaultParameters) {
    Vector2 pos(0.0, 0.0);
    Particle p(pos);

    EXPECT_EQ(p.getPosition(), pos);
    EXPECT_DOUBLE_EQ(p.getMass(), 1.0);
    EXPECT_DOUBLE_EQ(p.getRadius(), 1.0);
    EXPECT_FALSE(p.isPinned());
}

TEST(ParticleTest, InverseMass) {
    Particle movable(Vector2(0, 0), 2.0);
    Particle immovable(Vector2(0, 0), 0.0);

    EXPECT_DOUBLE_EQ(movable.getInvMass(), 0.5);
    EXPECT_DOUBLE_EQ(immovable.getInvMass(), 0.0);
}

// --------------------------------------------------
// Force Accumulation
// --------------------------------------------------

TEST(ParticleTest, ApplyForceAccumulates) {
    Particle p(Vector2(0, 0), 1.0);

    p.applyForce(Vector2(1, 0));
    p.applyForce(Vector2(0, 2));

    // One update step to expose the effect of force_accum
    p.update(1.0);

    // a = F / m = (1,2)
    // position += acceleration * dt^2
    EXPECT_EQ(p.getPosition(), Vector2(1, 2));
}

// --------------------------------------------------
// Update / Integration
// --------------------------------------------------

TEST(ParticleTest, UpdateMovesParticleWithNoInitialVelocity) {
    Particle p(Vector2(0, 0), 1.0);
    p.applyForce(Vector2(2, 0));

    p.update(1.0);

    // position = 0 + (0 - 0) + (2 * 1^2)
    EXPECT_EQ(p.getPosition(), Vector2(2, 0));
    EXPECT_EQ(p.getPrevPosition(), Vector2(0, 0));
}

TEST(ParticleTest, VerletIntegrationUsesPreviousPosition) {
    Particle p(Vector2(1, 0), 1.0);

    // Simulate initial velocity by modifying prev_position
    p.setPrevPosition(Vector2(0, 0));

    // No force applied -> constant velocity
    p.update(1.0);

    // position += (1 - 0) = 1 -> new position = 2
    EXPECT_EQ(p.getPosition(), Vector2(2, 0));
    EXPECT_EQ(p.getPrevPosition(), Vector2(1, 0));
}

TEST(ParticleTest, ForceIsClearedAfterUpdate) {
    Particle p(Vector2(0, 0), 1.0);
    p.applyForce(Vector2(1, 0));

    p.update(1.0);
    Vector2 posAfterFirst = p.getPosition();

    // No more force -> continues with same velocity
    p.update(1.0);

    EXPECT_EQ(p.getPosition(), posAfterFirst + (posAfterFirst - Vector2(0, 0)));
}

// --------------------------------------------------
// Pinned Behavior
// --------------------------------------------------

TEST(ParticleTest, PinnedParticleDoesNotMove) {
    Particle p(Vector2(5, 5), 1.0, 1.0, true);

    p.applyForce(Vector2(100, 100));
    p.update(1.0);
    p.update(1.0);

    EXPECT_EQ(p.getPosition(), Vector2(5, 5));
    EXPECT_EQ(p.getPrevPosition(), Vector2(5, 5));
    EXPECT_TRUE(p.isPinned());
}

// --------------------------------------------------
// Setters
// --------------------------------------------------

TEST(ParticleTest, SettersWorkCorrectly) {
    Particle p(Vector2(0, 0));

    p.setPosition(Vector2(3, 4));
    p.setPrevPosition(Vector2(1, 2));

    EXPECT_EQ(p.getPosition(), Vector2(3, 4));
    EXPECT_EQ(p.getPrevPosition(), Vector2(1, 2));
}
