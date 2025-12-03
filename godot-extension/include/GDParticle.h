//GDParticle.h
#pragma once
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include "Particle.h"
#include "GDVector2.h"

namespace godot {

    class GDParticle : public Resource {
        GDCLASS(GDParticle, Resource)

    private:
        // Editor-facing parameters
        Vector2 position_gd;
        double mass = 1.0;
        bool pinned = false;

        // Internal simulation object (allocated on build)
        sim::Particle* particle = nullptr;

    protected:
        static void _bind_methods();

    public:
        GDParticle() {}
        ~GDParticle();

        void build();
        void reset();

        // getters / setters
        void set_position(const Vector2 &p) { position_gd = p; }
        Vector2 get_position() const { return position_gd; }

        void set_mass(double m) { mass = m; }
        double get_mass() const { return mass; }

        void set_pinned(bool b) { pinned = b; }
        bool is_pinned() const { return pinned; }

        // Access to the sim object
        sim::Particle* get_sim_particle() const { return particle; }
        sim::Particle* take_sim_particle() {
            sim::Particle* p = particle;
            particle = nullptr;
            return p;
        }
    };

}