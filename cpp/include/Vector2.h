#pragma once
#include <cmath>
#include <iostream>

namespace sim {
    struct Vector2 {
        double x, y;

        Vector2(double x = 0.0f, double y = 0.0f) : x(x), y(y) {}

        Vector2 operator+(const Vector2& other) const { return {x + other.x, y + other.y}; }
        Vector2 operator-() const { return {-x, -y}; }
        Vector2 operator-(double scalar) const { return {x-scalar, y-scalar}; }
        Vector2 operator-(const Vector2& other) const { return {x - other.x, y - other.y}; }
        Vector2 operator*(double scalar) const { return {x * scalar, y * scalar}; }
        Vector2 operator/(double scalar) const { return {x / scalar, y / scalar}; }

        Vector2& operator+=(const Vector2& other) { x += other.x; y += other.y; return *this; }
        Vector2& operator-=(const Vector2& other) { x -= other.x; y -= other.y; return *this; }
        Vector2& operator*=(double scalar) { x *= scalar; y *= scalar; return *this; }
        Vector2& operator/=(double scalar) { x /= scalar; y /= scalar; return *this; }

        bool operator==(const Vector2& other) const { return x == other.x && y == other.y; }
        bool operator!=(const Vector2& other) const { return !(*this == other); }

        double dot(const Vector2& other) const { return x * other.x + y * other.y; }
        double cross(const Vector2& other) const { return x * other.y - y * other.x; }
        double lengthSquared() const { return this->dot(*this); }
        double length() const { return std::sqrt(lengthSquared()); }

        Vector2 normalized() const {
            double len = length();
            return (len != 0.0f) ? (*this / len) : Vector2(0.0f, 0.0f);
        }

        Vector2 abs() const {
            Vector2 out(std::abs(x),std::abs(y));
            return out;
        }

        friend Vector2 operator*(double scalar, const Vector2& v) { return {v.x * scalar, v.y * scalar}; }
        friend std::ostream& operator<<(std::ostream& os, const Vector2& v) {
            return os << "(" << v.x << ", " << v.y << ")";
        }
    };
}