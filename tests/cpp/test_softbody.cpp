#include <gtest/gtest.h>

#include "SoftBody.h"

using sim::SoftBody;
using sim::Particle;
using sim::Constraint;
using sim::Vector2;
using sim::divideSegment;

// --------------------------------------------------
// divideSegment
// --------------------------------------------------

TEST(DivideSegmentTest, SamePointReturnsSinglePoint) {
    Vector2 p(1, 1);
    auto pts = divideSegment(p, p, 5);

    ASSERT_EQ(pts.size(), 1);
    EXPECT_EQ(pts[0], p);
}

TEST(DivideSegmentTest, ZeroOrNegativeNReturnsEndpoints) {
    Vector2 a(0, 0);
    Vector2 b(10, 0);

    auto pts0 = divideSegment(a, b, 0);
    auto ptsNeg = divideSegment(a, b, -3);

    EXPECT_EQ(pts0.size(), 2);
    EXPECT_EQ(ptsNeg.size(), 2);

    EXPECT_EQ(pts0[0], a);
    EXPECT_EQ(pts0[1], b);
}

TEST(DivideSegmentTest, DividesIntoEqualSegments) {
    Vector2 a(0, 0);
    Vector2 b(10, 0);

    auto pts = divideSegment(a, b, 5);

    ASSERT_EQ(pts.size(), 6);
    EXPECT_EQ(pts.front(), a);
    EXPECT_EQ(pts.back(), b);

    EXPECT_NEAR(pts[1].x, 2.0, 1e-8);
    EXPECT_NEAR(pts[2].x, 4.0, 1e-8);
}

// --------------------------------------------------
// SoftBody Construction
// --------------------------------------------------

TEST(SoftBodyTest, ConstructorStoresData) {
    Particle p1(Vector2(0, 0));
    Particle p2(Vector2(1, 0));
    Constraint c(&p1, &p2);

    std::vector<Particle*> particles{ &p1, &p2 };
    std::vector<Constraint*> constraints{ &c };

    SoftBody body(particles, constraints, 0.3, 0.7);

    EXPECT_EQ(body.getParticles().size(), 2);
    EXPECT_EQ(body.getConstraints().size(), 1);
    EXPECT_DOUBLE_EQ(body.getFriction(), 0.3);
    EXPECT_DOUBLE_EQ(body.getRestitution(), 0.7);
}

// --------------------------------------------------
// SoftBody Behavior
// --------------------------------------------------

TEST(SoftBodyTest, ApplyForceAffectsAllParticles) {
    Particle p1(Vector2(0, 0));
    Particle p2(Vector2(0, 0));

    std::vector<Particle*> particles{ &p1, &p2 };
    SoftBody body(particles);

    body.applyForce(Vector2(1, 2));
    body.update(1.0);

    EXPECT_EQ(p1.getPosition(), Vector2(1, 2));
    EXPECT_EQ(p2.getPosition(), Vector2(1, 2));
}

TEST(SoftBodyTest, SolveConstraintCallsAllConstraints) {
    Particle p1(Vector2(0, 0));
    Particle p2(Vector2(10, 0));

    Constraint c(&p1, &p2, 1.0, 0.0);

    std::vector<Particle*> particles{ &p1, &p2 };
    std::vector<Constraint*> constraints{ &c };
    SoftBody body(particles, constraints);

    // Disturb constraint
    p2.setPosition(Vector2(12, 0));
    body.solveConstraint();

    EXPECT_NEAR(p1.getPosition().x, 1.0, 1e-8);
    EXPECT_NEAR(p2.getPosition().x, 11.0, 1e-8);
}

TEST(SoftBodyTest, UpdateAdvancesAllParticles) {
    Particle p1(Vector2(0, 0));
    Particle p2(Vector2(1, 0));

    std::vector<Particle*> particles{ &p1, &p2 };
    SoftBody body(particles);

    body.applyForce(Vector2(1, 0));
    body.update(1.0);

    EXPECT_EQ(p1.getPosition(), Vector2(1, 0));
    EXPECT_EQ(p2.getPosition(), Vector2(2, 0));
}

// --------------------------------------------------
// SoftBody Factory (High-level Sanity Tests)
// --------------------------------------------------

TEST(SoftBodyFactoryTest, CreateFromTriangle) {
    std::vector<Vector2> triangle = {
        Vector2(0, 0),
        Vector2(100, 0),
        Vector2(50, 100)
    };

    SoftBody* body = SoftBody::createFromPolygon(
        triangle,
        /*mesh_unit=*/5,
        /*mass=*/1.0,
        /*radius=*/1.0
    );

    ASSERT_NE(body, nullptr);
    EXPECT_FALSE(body->getParticles().empty());
    EXPECT_FALSE(body->getConstraints().empty());
    EXPECT_EQ(body->getBorder().size(), triangle.size());

    for (auto p: body->getParticles()) delete p;
    for (auto c: body->getConstraints()) delete c;
    delete body;
}

TEST(SoftBodyFactoryTest, AllBorderParticlesMatchPolygon) {
    std::vector<Vector2> square = {
        Vector2(0, 0),
        Vector2(10, 0),
        Vector2(10, 10),
        Vector2(0, 10)
    };

    SoftBody* body = SoftBody::createFromPolygon(square, 5);

    auto border = body->getBorder();
    ASSERT_EQ(border.size(), square.size());

    for (int i = 0; i < square.size(); ++i) {
        EXPECT_EQ(border[i]->getPosition(), square[i]);
    }

    for (auto p: body->getParticles()) delete p;
    for (auto c: body->getConstraints()) delete c;
    delete body;
}
