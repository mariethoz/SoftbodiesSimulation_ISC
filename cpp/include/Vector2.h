#pragma once
#include <cmath>
#include <iostream>

struct Vector2 {
    float x, y;

    Vector2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

    Vector2 operator+(const Vector2& other) const { return {x + other.x, y + other.y}; }
    Vector2 operator-() const { return {-x, -y}; }
    Vector2 operator-(const Vector2& other) const { return {x - other.x, y - other.y}; }
    Vector2 operator*(float scalar) const { return {x * scalar, y * scalar}; }
    Vector2 operator/(float scalar) const { return {x / scalar, y / scalar}; }

    Vector2& operator+=(const Vector2& other) { x += other.x; y += other.y; return *this; }
    Vector2& operator-=(const Vector2& other) { x -= other.x; y -= other.y; return *this; }
    Vector2& operator*=(float scalar) { x *= scalar; y *= scalar; return *this; }
    Vector2& operator/=(float scalar) { x /= scalar; y /= scalar; return *this; }

    bool operator==(const Vector2& other) const { return x == other.x && y == other.y; }
    bool operator!=(const Vector2& other) const { return !(*this == other); }

    float dot(const Vector2& other) const { return x * other.x + y * other.y; }
    float cross(const Vector2& other) const { return x * other.y - y * other.x; }
    float lengthSquared() const { return this->dot(*this); }
    float length() const { return std::sqrt(lengthSquared()); }

    Vector2 normalized() const {
        float len = length();
        return (len != 0.0f) ? (*this / len) : Vector2(0.0f, 0.0f);
    }

    friend Vector2 operator*(float scalar, const Vector2& v) { return {v.x * scalar, v.y * scalar}; }
    friend std::ostream& operator<<(std::ostream& os, const Vector2& v) {
        return os << "(" << v.x << ", " << v.y << ")";
    }
};
