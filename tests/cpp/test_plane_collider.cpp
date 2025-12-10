#include "PlaneWorldCollider.h"
#include "Particle.h"
#include "Vector2.h"
#include <gtest/gtest.h>

using namespace sim;

static void expectVecNear(const Vector2& a, const Vector2& b, float tol = 1e-5f) {
    EXPECT_NEAR(a.x, b.x, tol);
    EXPECT_NEAR(a.y, b.y, tol);
}

TEST(PlaneColliderTest, ConstructorSetsNormalAndDistance) {
    PlaneCollider plane(Vector2(0.0f, 1.0f), 5.0f);
    expectVecNear(plane.getNormal(), Vector2(0.0f, 1.0f));
    EXPECT_FLOAT_EQ(plane.getDistance(), 5.0f);
}

TEST(PlaneColliderTest, ParticleAbovePlaneNoCollision) {
    PlaneCollider plane(Vector2(0.0f, 1.0f), 0.0f);
    Particle p(Vector2(0.0f, 1.0f), 1.0f);

    EXPECT_FALSE(plane.collide(&p));
}

TEST(PlaneColliderTest, ParticleBelowPlaneCollides) {
    PlaneCollider plane(Vector2(0.0f, 1.0f), 0.0f);
    Particle p(Vector2(0.0f, -1.0f), 1.0f);

    EXPECT_TRUE(plane.collide(&p));
}
