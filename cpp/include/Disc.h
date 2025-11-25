#pragma once
#include "Body.h"
#include "Particle.h"

class Disc : public Body {
public:
    Disc(Particle center, double radius = 1.0);

    void applyForce(Vector2& f) override;
    void update(double dt) override;

    Vector2 getPosition() const override { return center.getPosition(); }
    Vector2 getVelocity() const {return center.getVelocity(); }
    void setVelocity(Vector2 v) {return center.setVelocity(v); }
    double getRadius() const { return radius; }
    double getMass() const {return center.getMass(); }

    bool intersects(const Body& other) const override;

private:
    Particle center;
    double radius;
};