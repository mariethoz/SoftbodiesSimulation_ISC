#pragma once
#include <cmath>
#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace sim {
    /**
     * @brief A 2D vector class providing basic arithmetic, geometric operations, and JSON serialization.
     *
     * This struct represents a mathematical vector in 2D space with components (x, y).
     * It supports vector arithmetic, normalization, dot/cross products, and serialization
     * to/from JSON for persistence.
     */
    struct Vector2 {
        double x, y;    /// The x and y components of the vector.

        Vector2(double x = 0.0, double y = 0.0) : x(x), y(y) {}

        Vector2 operator+(const Vector2& other) const { return {x + other.x, y + other.y}; }
        Vector2 operator-() const { return {-x, -y}; }
        Vector2 operator-(const Vector2& other) const { return {x - other.x, y - other.y}; }
        Vector2 operator*(double scalar) const { return {x * scalar, y * scalar}; }
        Vector2 operator/(double scalar) const { return {x / scalar, y / scalar}; }

        Vector2& operator+=(const Vector2& other) { x += other.x; y += other.y; return *this; }
        Vector2& operator-=(const Vector2& other) { x -= other.x; y -= other.y; return *this; }
        Vector2& operator*=(double scalar) { x *= scalar; y *= scalar; return *this; }
        Vector2& operator/=(double scalar) { x /= scalar; y /= scalar; return *this; }

        bool operator==(const Vector2& other) const {
            return std::abs(x - other.x) < 1e-8 && std::abs(y - other.y) < 1e-8;
        }
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
            return {std::abs(x), std::abs(y)};
        }

        friend Vector2 operator*(double scalar, const Vector2& v) { return {v.x * scalar, v.y * scalar}; }
        friend std::ostream& operator<<(std::ostream& os, const Vector2& v) {
            return os << "(" << v.x << ", " << v.y << ")";
        }

        // --- Saver & Loader ----
        json as_json() {
            json data;
            data["x"] = x;
            data["y"] = x;
            return data;
        }
        static Vector2 from_json(json data) {
            return Vector2(data["x"],data["y"]);
        }
    };

    /**
     * @brief Compute Euclidean distance between two vectors.
     * @param a First vector.
     * @param b Second vector.
     * @return Distance between a and b.
     */
    static double dist(const Vector2& a, const Vector2& b) {
        return (a - b).length();
    };

    /**
     * @brief Linearly interpolate between two vectors.
     * @param A Start vector.
     * @param B End vector.
     * @param t Interpolation factor (0.0 = A, 1.0 = B).
     * @return Interpolated vector.
     */
    static Vector2 interpolate(const Vector2& A, const Vector2& B, double t) {
        return A + (B - A) * t;
    };
}