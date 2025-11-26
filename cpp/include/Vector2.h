#pragma once
#include <cmath>

struct Vector2 {
    float x, y;

    Vector2(float x = 0.0, float y = 0.0) : x(x), y(y) {}

    Vector2 operator+(const Vector2& other) const { return {x + other.x, y + other.y}; }
    Vector2 operator-() const { return {-x, -y}; }
    Vector2 operator-(const Vector2& other) const { return {x - other.x, y - other.y}; }
    Vector2 operator*(float scalar) const { return {x * scalar, y * scalar}; }
    Vector2 operator/(float scalar) const { return {x / scalar, y / scalar}; }

    Vector2& operator+=(const Vector2& other) { x += other.x; y += other.y; return *this; }
    Vector2& operator-=(const Vector2& other) { x -= other.x; y -= other.y; return *this; }
    Vector2& operator*=(float scalar) { x *= scalar; y *= scalar; return *this; }
    Vector2& operator/=(float scalar) { x /= scalar; y /= scalar; return *this; }

    float dot(const Vector2& other) const { return x * other.x + y * other.y; }
    float length() const { return std::sqrt(this->dot(*this)); }
    Vector2 normalized() const {
        float len = length();
        return (len != 0.0) ? (*this / len) : Vector2(0.0, 0.0);
    }
};
