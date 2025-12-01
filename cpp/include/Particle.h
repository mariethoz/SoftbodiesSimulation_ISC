#pragma once
#include "Vector2.h"

namespace sim {
    class Particle {
    public:
        Particle(Vector2 pos, float mass = 1.0f, bool pinned = false);

        // Core function
        void applyForce(const Vector2& f);
        void update(double dt);

        // --- Accessors & mutators ----
        const Vector2& getPosition() const { return position; }
        const Vector2& getPrevPosition() const { return prev_position; }
        void setPosition(const Vector2& p) { position = p; }
        void setPrevPosition(const Vector2& p) { prev_position = p; }

        float getRadius() const { return radius; }
        float getMass() const { return mass; }
        // inverse mass: 0 means immovable
        float getInvMass() const { return (mass <= 0.0f) ? 0.0f : (1.0f / mass); }

        bool isPinned() const { return pinned; }

    private:
        Vector2 position;
        Vector2 prev_position;
        Vector2 force_accum;
        float radius;
        float mass;
        bool pinned;
    };
}