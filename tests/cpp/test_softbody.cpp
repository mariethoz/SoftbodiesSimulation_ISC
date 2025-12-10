#include "SoftBody.h"
#include "Vector2.h"
#include <gtest/gtest.h>

using namespace sim;

// Helper: equality check for Vector2 with tolerance
static void expectVecNear(const Vector2& a, const Vector2& b, float tol = 1e-5f) {
    EXPECT_NEAR(a.x, b.x, tol);
    EXPECT_NEAR(a.y, b.y, tol);
}

TEST(SoftBodyTest, ConstructorInitializesParticles) {
    std::vector<Particle*> particles;
    particles.push_back(new Particle(Vector2(0.0f, 0.0f), 1.0f));
    particles.push_back(new Particle(Vector2(1.0f, 0.0f), 2.0f));

    SoftBody body(particles);

    auto ps = body.getParticles();
    EXPECT_EQ(ps.size(), 2u);
    expectVecNear(ps[0]->getPosition(), Vector2(0.0f, 0.0f));
    expectVecNear(ps[1]->getPosition(), Vector2(1.0f, 0.0f));
    EXPECT_FLOAT_EQ(body.getFriction(), 0.5f);
    EXPECT_FLOAT_EQ(body.getRestitution(), 0.5f);

    for (auto p: particles) {
        delete p;
    }
}

TEST(SoftBodyTest, ConstructorWithFrictionAndRestitution) {
    std::vector<Particle*> particles;
    particles.push_back(new Particle(Vector2(0.0f, 0.0f), 1.0f));

    SoftBody body(particles, {}, 0.3f, 0.8f);

    EXPECT_FLOAT_EQ(body.getFriction(), 0.3f);
    EXPECT_FLOAT_EQ(body.getRestitution(), 0.8f);

    for (auto p: particles) {
        delete p;
    }
}

TEST(SoftBodyTest, ApplyForcePropagatesToAllParticles) {
    std::vector<Particle*> particles;
    particles.push_back(new Particle(Vector2(0.0f, 0.0f), 1.0f));
    particles.push_back(new Particle(Vector2(1.0f, 0.0f), 1.0f));

    SoftBody body(particles);

    body.applyForce(Vector2(2.0f, 0.0f));
    body.update(1.0);

    auto ps = body.getParticles();
    expectVecNear(ps[0]->getPosition(), Vector2(2.0f, 0.0f));
    expectVecNear(ps[1]->getPosition(), Vector2(3.0f, 0.0f));

    for (auto p: particles) {
        delete p;
    }
}

TEST(SoftBodyTest, UpdateMovesAllParticles) {
    std::vector<Particle*> particles;
    particles.emplace_back(new Particle(Vector2(0.0f, 0.0f), 1.0f));
    particles.emplace_back(new Particle(Vector2(1.0f, 0.0f), 1.0f));

    SoftBody body(particles);

    // Apply upward force
    body.applyForce(Vector2(0.0f, 5.0f));
    body.update(1.0);

    auto ps = body.getParticles();
    expectVecNear(ps[0]->getPosition(), Vector2(0.0f, 5.0f));
    expectVecNear(ps[1]->getPosition(), Vector2(1.0f, 5.0f));

    for (auto p: particles) {
        delete p;
    }
}
