#pragma once

struct Vector2 {
    double x, y;

    Vector2(double x = 0.0, double y = 0.0) : x(x), y(y) {}

    Vector2 operator+(const Vector2& other) const { return {x + other.x, y + other.y}; }
    Vector2 operator-(const Vector2& other) const { return {x - other.x, y - other.y}; }
    Vector2 operator*(double scalar) const { return {x * scalar, y * scalar}; }
    Vector2 operator/(double scalar) const { return {x / scalar, y / scalar}; }

    Vector2& operator+=(const Vector2& other) { x += other.x; y += other.y; return *this; }
    Vector2& operator*=(double scalar) { x *= scalar; y *= scalar; return *this; }

    double dot(Vector2 other) const {return x*other.x + y*other.y;}
};
