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

    EXPECT_FALSE(inner.collide(p));
}

TEST(InnerCircleColliderTest, ParticleOutsideCollides) {
    InnerCircleCollider inner(Vector2(0.0f, 0.0f), 5.0f);
    Particle p(Vector2(10.0f, 0.0f), 1.0f);

    EXPECT_TRUE(inner.collide(p));
}

TEST(OuterCircleColliderTest, ParticleOutsideNoCollision) {
    OuterCircleCollider outer(Vector2(0.0f, 0.0f), 5.0f);
    Particle p(Vector2(10.0f, 0.0f), 1.0f);

    EXPECT_FALSE(outer.collide(p));
}

TEST(OuterCircleColliderTest, ParticleInsideCollides) {
    OuterCircleCollider outer(Vector2(0.0f, 0.0f), 5.0f);
    Particle p(Vector2(1.0f, 1.0f), 1.0f);

    EXPECT_TRUE(outer.collide(p));
}
