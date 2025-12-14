#include <gtest/gtest.h>

#include "Constraint.h"

using sim::Constraint;
using sim::Particle;
using sim::Vector2;

// --------------------------------------------------
// Construction
// --------------------------------------------------

TEST(ConstraintTest, ConstructorComputesRestLength) {
    Particle p1(Vector2(0, 0));
    Particle p2(Vector2(3, 4));

    Constraint c(&p1, &p2);

    EXPECT_DOUBLE_EQ(c.getRestLength(), 5.0);
}

TEST(ConstraintTest, InitialPositionsUnchanged) {
    Particle p1(Vector2(1, 2));
    Particle p2(Vector2(4, 6));

    Constraint c(&p1, &p2);
    c.applyConstraint();

    EXPECT_EQ(p1.getPosition(), Vector2(1, 2));
    EXPECT_EQ(p2.getPosition(), Vector2(4, 6));
}

// --------------------------------------------------
// Constraint Correction
// --------------------------------------------------

TEST(ConstraintTest, ConstraintPullsParticlesTogetherWhenTooFar) {
    Particle p1(Vector2(0, 0));
    Particle p2(Vector2(10, 0));

    Constraint c(&p1, &p2, 1.0, 0.0);

    // Move p2 farther away
    p2.setPosition(Vector2(12, 0));

    c.applyConstraint();

    // Expected symmetric correction
    EXPECT_NEAR(p1.getPosition().x, 1.0, 1e-8);
    EXPECT_NEAR(p2.getPosition().x, 11.0, 1e-8);
}

TEST(ConstraintTest, ConstraintPushesParticlesApartWhenTooClose) {
    Particle p1(Vector2(0, 0));
    Particle p2(Vector2(10, 0));

    Constraint c(&p1, &p2, 1.0, 0.0);

    // Move p2 closer
    p2.setPosition(Vector2(8, 0));

    c.applyConstraint();

    EXPECT_NEAR(p1.getPosition().x, -1.0, 1e-8);
    EXPECT_NEAR(p2.getPosition().x, 9.0, 1e-8);
}

// --------------------------------------------------
// Pinned Behavior
// --------------------------------------------------

TEST(ConstraintTest, PinnedParticleDoesNotMove) {
    Particle pinned(Vector2(0, 0), 1.0, 1.0, true);
    Particle free(Vector2(10, 0));

    Constraint c(&pinned, &free, 1.0, 0.0);

    free.setPosition(Vector2(12, 0));
    c.applyConstraint();

    EXPECT_EQ(pinned.getPosition(), Vector2(0, 0));
    EXPECT_NEAR(free.getPosition().x, 11.0, 1e-8);
}

TEST(ConstraintTest, BothParticlesPinnedNoMovement) {
    Particle p1(Vector2(0, 0), 1.0, 1.0, true);
    Particle p2(Vector2(10, 0), 1.0, 1.0, true);

    Constraint c(&p1, &p2);

    p2.setPosition(Vector2(12, 0));
    c.applyConstraint();

    EXPECT_EQ(p1.getPosition(), Vector2(0, 0));
    EXPECT_EQ(p2.getPosition(), Vector2(12, 0));
}

// --------------------------------------------------
// Degenerate Case
// --------------------------------------------------

TEST(ConstraintTest, ZeroDistanceDoesNothing) {
    Particle p1(Vector2(0, 0));
    Particle p2(Vector2(1, 0));

    Constraint c(&p1, &p2);

    p2.setPosition(Vector2(0,0));
    c.applyConstraint();

    EXPECT_EQ(p1.getPosition(), Vector2(0, 0));
    EXPECT_EQ(p2.getPosition(), Vector2(0, 0));
}

// --------------------------------------------------
// Damping
// --------------------------------------------------

TEST(ConstraintTest, DampingModifiesPreviousPosition) {
    Particle p1(Vector2(0, 0));
    Particle p2(Vector2(10, 0));

    // Simulate velocity
    p1.setPrevPosition(Vector2(-1, 0));
    p2.setPrevPosition(Vector2(11, 0));

    Constraint c(&p1, &p2, 0.0, 1.0);

    c.applyConstraint();

    // Damping should reduce relative velocity
    EXPECT_NE(p1.getPrevPosition(), Vector2(-1, 0));
    EXPECT_NE(p2.getPrevPosition(), Vector2(11, 0));
}

// --------------------------------------------------
// Accessors
// --------------------------------------------------

TEST(ConstraintTest, AccessorsReturnParticlePositions) {
    Particle p1(Vector2(1, 1));
    Particle p2(Vector2(2, 2));

    Constraint c(&p1, &p2);

    EXPECT_EQ(c.getPart1(), Vector2(1, 1));
    EXPECT_EQ(c.getPart2(), Vector2(2, 2));
}
