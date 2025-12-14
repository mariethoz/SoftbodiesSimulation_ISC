#include <gtest/gtest.h>

#include "PlaneWorldCollider.h"
#include "CircleWorldCollider.h"

using sim::Particle;
using sim::Vector2;
using sim::PlaneCollider;
using sim::InnerCircleCollider;
using sim::OuterCircleCollider;

// --------------------------------------------------
// PlaneCollider
// --------------------------------------------------

TEST(PlaneColliderTest, NoCollisionWhenAbovePlane) {
    PlaneCollider plane(Vector2(0, 1), 0.0);
    Particle p(Vector2(0, 10), 1.0, 1.0);

    bool collided = plane.collide(&p, 0.5, 0.5);

    EXPECT_FALSE(collided);
    EXPECT_EQ(p.getPosition(), Vector2(0, 10));
}

TEST(PlaneColliderTest, CollisionPushesParticleOutOfPlane) {
    PlaneCollider plane(Vector2(0, 1), 0.0);
    Particle p(Vector2(0, 0.2), 1.0, 1.0);

    p.setPrevPosition(Vector2(0, 1.0)); // downward velocity

    bool collided = plane.collide(&p, 0.5, 0.5);

    EXPECT_TRUE(collided);
    EXPECT_NEAR(p.getPosition().y, 1.0, 1e-8);
}

TEST(PlaneColliderTest, PlaneDoesNotAffectPinnedParticle) {
    PlaneCollider plane(Vector2(0, 1), 0.0);
    Particle p(Vector2(0, -1), 1.0, 1.0, true);

    bool collided = plane.collide(&p, 0.5, 0.5);

    EXPECT_FALSE(collided);
    EXPECT_EQ(p.getPosition(), Vector2(0, -1));
}

TEST(PlaneColliderTest, FrictionReducesTangentialVelocity) {
    PlaneCollider plane(Vector2(0, 1), 0.0, 1.0, 0.0);
    Particle p(Vector2(0, 0.5), 1.0, 1.0);

    // Simulate diagonal velocity
    p.setPrevPosition(Vector2(-1, 1));

    plane.collide(&p, 1.0, 0.0);

    Vector2 newVel = p.getPosition() - p.getPrevPosition();

    EXPECT_NEAR(newVel.x, 0.0, 1e-6); // friction removes tangent motion
}

// --------------------------------------------------
// InnerCircleCollider
// --------------------------------------------------

TEST(InnerCircleColliderTest, NoCollisionWhenInsideBoundary) {
    InnerCircleCollider circle(Vector2(0, 0), 10.0);
    Particle p(Vector2(0, 0), 1.0, 1.0);

    bool collided = circle.collide(&p, 0.5, 0.5);

    EXPECT_FALSE(collided);
}

TEST(InnerCircleColliderTest, CollisionKeepsParticleInside) {
    InnerCircleCollider circle(Vector2(0, 0), 10.0);
    Particle p(Vector2(10, 0), 1.0, 1.0);

    p.setPrevPosition(Vector2(9, 0)); // outward velocity

    bool collided = circle.collide(&p, 0.5, 0.5);

    EXPECT_TRUE(collided);
    EXPECT_NEAR(p.getPosition().x, 9.0, 1e-8);
}

TEST(InnerCircleColliderTest, HandlesZeroDistanceSafely) {
    InnerCircleCollider circle(Vector2(0, 0), 5.0);
    Particle p(Vector2(0, 0), 1.0, 1.0);

    bool collided = circle.collide(&p, 0.5, 0.5);

    EXPECT_FALSE(collided);
}

// --------------------------------------------------
// OuterCircleCollider
// --------------------------------------------------

TEST(OuterCircleColliderTest, NoCollisionWhenOutsideBoundary) {
    OuterCircleCollider circle(Vector2(0, 0), 5.0);
    Particle p(Vector2(10, 0), 1.0, 1.0);

    bool collided = circle.collide(&p, 0.5, 0.5);

    EXPECT_FALSE(collided);
}

TEST(OuterCircleColliderTest, CollisionPushesParticleOutside) {
    OuterCircleCollider circle(Vector2(0, 0), 5.0);
    Particle p(Vector2(4.5, 0), 1.0, 1.0);

    p.setPrevPosition(Vector2(5.5, 0)); // inward velocity

    bool collided = circle.collide(&p, 0.5, 0.5);

    EXPECT_TRUE(collided);
    EXPECT_NEAR(p.getPosition().x, 6.0, 1e-8);
}

TEST(OuterCircleColliderTest, PinnedParticleIgnored) {
    OuterCircleCollider circle(Vector2(0, 0), 5.0);
    Particle p(Vector2(4, 0), 1.0, 1.0, true);

    bool collided = circle.collide(&p, 0.5, 0.5);

    EXPECT_FALSE(collided);
    EXPECT_EQ(p.getPosition(), Vector2(4, 0));
}
