#pragma once
#include <nlohmann/json.hpp>

#include "Vector2.h"

using json = nlohmann::json;

namespace sim {
    class Particle {
    public:
        Particle(Vector2 pos, double mass = 1.0, double radius = 1.0, bool pinned = false);
        ~Particle();

        // Core function
        void applyForce(const Vector2& f);
        void update(double dt);

        // --- Accessors & mutators ----
        const Vector2& getPosition() const { return position; }
        const Vector2& getPrevPosition() const { return prev_position; }
        void setPosition(const Vector2& p) { position = p; }
        void setPrevPosition(const Vector2& p) { prev_position = p; }

        double getRadius() const { return radius; }
        double getMass() const { return mass; }
        // inverse mass: 0 means immovable
        double getInvMass() const { return (mass <= 0.0f) ? 0.0f : (1.0f / mass); }

        bool isPinned() const { return pinned; }

        // --- Saver & Loader ----
        json as_json();
        static Vector2 from_json(json data);

    private:
        Vector2 position;
        Vector2 prev_position;
        Vector2 force_accum;
        double radius;
        double mass;
        bool pinned;
    };
}