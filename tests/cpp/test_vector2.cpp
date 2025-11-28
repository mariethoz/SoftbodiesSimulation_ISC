#include "Vector2.h"
#include <gtest/gtest.h>

using namespace sim;

TEST(Vector2, Add) {
    Vector2 a(1, 2);
    Vector2 b(3, 4);
    Vector2 c = a + b;
    EXPECT_FLOAT_EQ(c.x, 4);
    EXPECT_FLOAT_EQ(c.y, 6);
}

TEST(Vector2, Subtract) {
    Vector2 a(5, 7);
    Vector2 b(2, 3);
    Vector2 c = a - b;
    EXPECT_FLOAT_EQ(c.x, 3);
    EXPECT_FLOAT_EQ(c.y, 4);
}

TEST(Vector2, UnaryMinus) {
    Vector2 a(1, -2);
    Vector2 b = -a;
    EXPECT_FLOAT_EQ(b.x, -1);
    EXPECT_FLOAT_EQ(b.y, 2);
}

TEST(Vector2, ScalarMultiplication) {
    Vector2 a(2, 3);
    Vector2 b = a * 2.0f;
    EXPECT_FLOAT_EQ(b.x, 4);
    EXPECT_FLOAT_EQ(b.y, 6);
}

TEST(Vector2, ScalarDivision) {
    Vector2 a(4, 8);
    Vector2 b = a / 2.0f;
    EXPECT_FLOAT_EQ(b.x, 2);
    EXPECT_FLOAT_EQ(b.y, 4);
}

TEST(Vector2, CompoundAdd) {
    Vector2 a(1, 2);
    Vector2 b(3, 4);
    a += b;
    EXPECT_FLOAT_EQ(a.x, 4);
    EXPECT_FLOAT_EQ(a.y, 6);
}

TEST(Vector2, CompoundSubtract) {
    Vector2 a(5, 6);
    Vector2 b(2, 3);
    a -= b;
    EXPECT_FLOAT_EQ(a.x, 3);
    EXPECT_FLOAT_EQ(a.y, 3);
}

TEST(Vector2, CompoundMultiply) {
    Vector2 a(2, 3);
    a *= 2.0f;
    EXPECT_FLOAT_EQ(a.x, 4);
    EXPECT_FLOAT_EQ(a.y, 6);
}

TEST(Vector2, CompoundDivide) {
    Vector2 a(4, 8);
    a /= 2.0f;
    EXPECT_FLOAT_EQ(a.x, 2);
    EXPECT_FLOAT_EQ(a.y, 4);
}

TEST(Vector2, EqualityOperators) {
    Vector2 a(1, 2);
    Vector2 b(1, 2);
    Vector2 c(2, 3);
    EXPECT_TRUE(a == b);
    EXPECT_TRUE(a != c);
}

TEST(Vector2, DotProduct) {
    Vector2 a(1, 2);
    Vector2 b(3, 4);
    EXPECT_FLOAT_EQ(a.dot(b), 11);
}

TEST(Vector2, CrossProduct) {
    Vector2 a(1, 2);
    Vector2 b(3, 4);
    // cross = a.x * b.y - a.y * b.x = 1*4 - 2*3 = -2
    EXPECT_FLOAT_EQ(a.cross(b), -2);
}

TEST(Vector2, LengthSquared) {
    Vector2 a(3, 4);
    EXPECT_FLOAT_EQ(a.lengthSquared(), 25);
}

TEST(Vector2, Length) {
    Vector2 a(3, 4);
    EXPECT_FLOAT_EQ(a.length(), 5);
}

TEST(Vector2, Normalize) {
    Vector2 v(3, 4);
    Vector2 n = v.normalized();
    EXPECT_NEAR(n.length(), 1.0f, 1e-5);
    EXPECT_NEAR(n.x, 0.6f, 1e-5);
    EXPECT_NEAR(n.y, 0.8f, 1e-5);
}

TEST(Vector2, NormalizeZeroVector) {
    Vector2 v(0, 0);
    Vector2 n = v.normalized();
    EXPECT_FLOAT_EQ(n.x, 0);
    EXPECT_FLOAT_EQ(n.y, 0);
}

TEST(Vector2, ScalarOnLeftMultiplication) {
    Vector2 a(2, 3);
    Vector2 b = 2.0f * a;
    EXPECT_FLOAT_EQ(b.x, 4);
    EXPECT_FLOAT_EQ(b.y, 6);
}

TEST(Vector2, StreamOutput) {
    Vector2 a(1.5f, -2.5f);
    std::ostringstream os;
    os << a;
    EXPECT_EQ(os.str(), "(1.5, -2.5)");
}
