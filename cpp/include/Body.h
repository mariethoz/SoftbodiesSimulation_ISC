#pragma once
#include "Vector2.h"

class Body {
public:
    virtual ~Body() = default;

    // Core physics interface
    virtual void applyForce(Vector2& f) = 0;
    virtual void update(double dt) = 0;

    // Geometry interface
    virtual Vector2 getPosition() const = 0;

    // Collision check (generic)
    virtual bool intersects(const Body& other) const = 0;
};
