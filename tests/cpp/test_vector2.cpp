#include <gtest/gtest.h>
#include "Vector2.h"

using sim::Vector2;
using sim::dist;
using sim::interpolate;

// --------------------------------------------------
// Constructor & Equality
// --------------------------------------------------

TEST(Vector2Test, DefaultConstructor) {
    Vector2 v;
    EXPECT_DOUBLE_EQ(v.x, 0.0);
    EXPECT_DOUBLE_EQ(v.y, 0.0);
}

TEST(Vector2Test, ParameterizedConstructor) {
    Vector2 v(3.5, -2.0);
    EXPECT_DOUBLE_EQ(v.x, 3.5);
    EXPECT_DOUBLE_EQ(v.y, -2.0);
}

TEST(Vector2Test, EqualityOperators) {
    Vector2 a(1.0, 2.0);
    Vector2 b(1.0 + 1e-9, 2.0 - 1e-9);
    Vector2 c(2.0, 3.0);

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
    EXPECT_TRUE(a != c);
}

// --------------------------------------------------
// Arithmetic Operators
// --------------------------------------------------

TEST(Vector2Test, Addition) {
    Vector2 a(1, 2);
    Vector2 b(3, 4);
    Vector2 result = a + b;

    EXPECT_EQ(result, Vector2(4, 6));
}

TEST(Vector2Test, Subtraction) {
    Vector2 a(5, 6);
    Vector2 b(2, 3);
    Vector2 result = a - b;

    EXPECT_EQ(result, Vector2(3, 3));
}

TEST(Vector2Test, UnaryMinus) {
    Vector2 v(3, -4);
    EXPECT_EQ(-v, Vector2(-3, 4));
}

TEST(Vector2Test, ScalarMultiplication) {
    Vector2 v(2, 3);

    EXPECT_EQ(v * 2.0, Vector2(4, 6));
    EXPECT_EQ(2.0 * v, Vector2(4, 6));
}

TEST(Vector2Test, ScalarDivision) {
    Vector2 v(4, 6);
    Vector2 result = v / 2.0;

    EXPECT_EQ(result, Vector2(2, 3));
}

// --------------------------------------------------
// Compound Assignment
// --------------------------------------------------

TEST(Vector2Test, CompoundAssignment) {
    Vector2 v(1, 2);
    v += Vector2(3, 4);
    EXPECT_EQ(v, Vector2(4, 6));

    v -= Vector2(1, 1);
    EXPECT_EQ(v, Vector2(3, 5));

    v *= 2.0;
    EXPECT_EQ(v, Vector2(6, 10));

    v /= 2.0;
    EXPECT_EQ(v, Vector2(3, 5));
}

// --------------------------------------------------
// Vector Math
// --------------------------------------------------

TEST(Vector2Test, DotProduct) {
    Vector2 a(1, 2);
    Vector2 b(3, 4);

    EXPECT_DOUBLE_EQ(a.dot(b), 11.0);
}

TEST(Vector2Test, CrossProduct) {
    Vector2 a(1, 2);
    Vector2 b(3, 4);

    EXPECT_DOUBLE_EQ(a.cross(b), -2.0);
}

TEST(Vector2Test, LengthAndLengthSquared) {
    Vector2 v(3, 4);

    EXPECT_DOUBLE_EQ(v.lengthSquared(), 25.0);
    EXPECT_DOUBLE_EQ(v.length(), 5.0);
}

TEST(Vector2Test, Normalization) {
    Vector2 v(3, 4);
    Vector2 n = v.normalized();

    EXPECT_NEAR(n.length(), 1.0, 1e-8);
    EXPECT_NEAR(n.x, 0.6, 1e-8);
    EXPECT_NEAR(n.y, 0.8, 1e-8);
}

TEST(Vector2Test, NormalizeZeroVector) {
    Vector2 v(0, 0);
    EXPECT_EQ(v.normalized(), Vector2(0, 0));
}

TEST(Vector2Test, Abs) {
    Vector2 v(-3, 4);
    EXPECT_EQ(v.abs(), Vector2(3, 4));
}

// --------------------------------------------------
// Free Functions
// --------------------------------------------------

TEST(Vector2Test, Distance) {
    Vector2 a(0, 0);
    Vector2 b(3, 4);

    EXPECT_DOUBLE_EQ(dist(a, b), 5.0);
}

TEST(Vector2Test, Interpolation) {
    Vector2 a(0, 0);
    Vector2 b(10, 10);

    EXPECT_EQ(interpolate(a, b, 0.0), a);
    EXPECT_EQ(interpolate(a, b, 1.0), b);
    EXPECT_EQ(interpolate(a, b, 0.5), Vector2(5, 5));
}
