#include "CircleWorldCollider.h"
#include "Particle.h"
#include "Vector2.h"
#include <gtest/gtest.h>

using namespace sim;

static void expectVecNear(const Vector2& a, const Vector2& b, float tol = 1e-5f) {
    EXPECT_NEAR(a.x, b.x, tol);
    EXPECT_NEAR(a.y, b.y, tol);
}

TEST(CircleColliderTest, ConstructorSetsCenterAndRadius) {
    InnerCircleCollider inner(Vector2(0.0f, 0.0f), 10.0f);
    expectVecNear(inner.getCenter(), Vector2(0.0f, 0.0f));
    EXPECT_FLOAT_EQ(inner.getRadius(), 10.0f);
    OuterCircleCollider outer(Vector2(0.0f, 0.0f), 10.0f);
    expectVecNear(outer.getCenter(), Vector2(0.0f, 0.0f));
    EXPECT_FLOAT_EQ(outer.getRadius(), 10.0f);
}

TEST(InnerCircleColliderTest, ParticleInsideNoCollision) {
    InnerCircleCollider inner(Vector2(0.0f, 0.0f), 5.0f);
    Particle p(Vector2(1.0f, 1.0f), 1.0f);

    EXPECT_FALSE(inner.collide(&p));
}

TEST(InnerCircleColliderTest, ParticleOutsideCollides) {
    InnerCircleCollider inner(Vector2(0.0f, 0.0f), 5.0f);
    Particle p(Vector2(10.0f, 0.0f), 1.0f);

    EXPECT_TRUE(inner.collide(&p));
}

TEST(OuterCircleColliderTest, ParticleOutsideNoCollision) {
    OuterCircleCollider outer(Vector2(0.0f, 0.0f), 5.0f);
    Particle p(Vector2(10.0f, 0.0f), 1.0f);

    EXPECT_FALSE(outer.collide(&p));
}

TEST(OuterCircleColliderTest, ParticleInsideCollides) {
    OuterCircleCollider outer(Vector2(0.0f, 0.0f), 5.0f);
    Particle p(Vector2(1.0f, 1.0f), 1.0f);

    EXPECT_TRUE(outer.collide(&p));
}

TEST(InnerCircleColliderTest, ParticleExactlyOnBoundaryNoCollision) {
    InnerCircleCollider inner(Vector2(0.0f, 0.0f), 5.0f);
    Particle p(Vector2(4.0f, 0.0f), 1.0f); // dist = 4, radius - particle radius = 4

    EXPECT_FALSE(inner.collide(&p));
    expectVecNear(Vector2(4.0f, 0.0f), p.getPosition());
}

TEST(OuterCircleColliderTest, ParticleExactlyOnBoundaryNoCollision) {
    OuterCircleCollider outer(Vector2(0.0f, 0.0f), 5.0f);
    Particle p(Vector2(6.0f, 0.0f), 1.0f); // dist = 6, radius + particle radius = 6

    EXPECT_FALSE(outer.collide(&p));
    expectVecNear(Vector2(6.0f, 0.0f), p.getPosition());
}

TEST(InnerCircleColliderTest, CollisionMovesParticleToBoundary) {
    InnerCircleCollider inner(Vector2(0.0f, 0.0f), 5.0f);
    Particle p(Vector2(10.0f, 0.0f), 1.0f);

    EXPECT_TRUE(inner.collide(&p));
    // Expected new position: radius - particle radius = 4
    expectVecNear(Vector2(4.0f, 0.0f), p.getPosition());
}

TEST(OuterCircleColliderTest, CollisionMovesParticleToBoundary) {
    OuterCircleCollider outer(Vector2(0.0f, 0.0f), 5.0f);
    Particle p1(Vector2(0.0f, 0.0f), 1.0f);
    Particle p2(Vector2(1.0f, 1.0f), 1.0f);

    EXPECT_TRUE(outer.collide(&p1));
    EXPECT_TRUE(outer.collide(&p2));
    // Expected new position: radius + particle radius = 6, along +x axis
    p1.update(1.0);
    p2.update(1.0);
    EXPECT_EQ(0,p1.getPosition().x); // default when particle and outer circle collide
    EXPECT_NE(0,p1.getPosition().y);
    EXPECT_NE(0,p2.getPosition().x);
    EXPECT_NE(0,p2.getPosition().y);
}

TEST(CircleColliderTest, DestructorPrintsMessage) {
    testing::internal::CaptureStdout();
    {
        CircleCollider* c = new InnerCircleCollider(Vector2(0.0f, 0.0f), 5.0f);
        delete c;
    }
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("CircleCollider destroyed"), std::string::npos);
}
