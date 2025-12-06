#include "Constraint.h"
#include "Particle.h"
#include "Vector2.h"
#include <gtest/gtest.h>

using namespace sim;

static void expectVecNear(const Vector2& a, const Vector2& b, float tol = 1e-5f) {
    EXPECT_NEAR(a.x, b.x, tol);
    EXPECT_NEAR(a.y, b.y, tol);
}

TEST(ConstraintTest, ConstructorSetsRestLength) {
    Particle p1(Vector2(0.0f, 0.0f));
    Particle p2(Vector2(3.0f, 4.0f)); // distance = 5
    Constraint c(&p1, &p2);

    EXPECT_FLOAT_EQ(c.getRestLength(), 5.0f);
}

TEST(ConstraintTest, NoCorrectionWhenAtRestLength) {
    Particle p1(Vector2(0.0f, 0.0f));
    Particle p2(Vector2(5.0f, 0.0f)); // distance = 5
    Constraint c(&p1, &p2);

    c.applyConstraint();
    // Positions should remain unchanged
    expectVecNear(p1.getPosition(), Vector2(0.0f, 0.0f));
    expectVecNear(p2.getPosition(), Vector2(5.0f, 0.0f));
}

TEST(ConstraintTest, CorrectionWhenTooFarApart) {
    Particle p1(Vector2(0.0f, 0.0f));
    Particle p2(Vector2(10.0f, 0.0f)); // rest length = 10
    Constraint c(&p1, &p2);

    // Move p2 farther away
    p2.setPosition(Vector2(15.0f, 0.0f));
    p2.setPrevPosition(Vector2(15.0f, 0.0f));
    c.applyConstraint();
    p2.update(0.1);

    // After correction, p2 should be pulled closer to p1
    EXPECT_LT(p2.getPosition().x, 15.0f);
}

TEST(ConstraintTest, CorrectionWhenTooClose) {
    Particle p1(Vector2(0.0f, 0.0f));
    Particle p2(Vector2(10.0f, 0.0f)); // rest length = 10
    Constraint c(&p1, &p2);

    // Move p2 closer
    p2.setPosition(Vector2(5.0f, 0.0f));
    p2.setPrevPosition(Vector2(5.0f, 0.0f));
    c.applyConstraint();
    p2.update(1);
    // After correction, p2 should be pushed farther away from p1
    EXPECT_GT(p2.getPosition().x, 5.0f);
}

TEST(ConstraintTest, PinnedParticleDoesNotMove) {
    Particle p1(Vector2(0.0f, 0.0f), 1.0f, true); // pinned
    Particle p2(Vector2(10.0f, 0.0f));
    Constraint c(&p1, &p2);

    // Move p2 closer
    p2.setPosition(Vector2(5.0f, 0.0f));
    c.applyConstraint();

    // p1 should remain pinned at origin
    expectVecNear(p1.getPosition(), Vector2(0.0f, 0.0f));
}

TEST(ConstraintTest, DestructorPrintsMessage) {
    testing::internal::CaptureStdout();
    {
        Particle p1(Vector2(0.0f, 0.0f));
        Particle p2(Vector2(1.0f, 0.0f));
        Constraint* c = new Constraint(&p1, &p2);
        delete c;
    }
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Constraint destroyed"), std::string::npos);
}
