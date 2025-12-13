//2_GDSoftBody.h
#pragma once
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/polygon2d.hpp>
#include <godot_cpp/core/binder_common.hpp>

#include "SoftBody.h"

#include "GDParticle.h"
#include "GDConstraint.h"

namespace godot {

    class GDSoftBody_2 : public Polygon2D {
        GDCLASS(GDSoftBody_2, Polygon2D)

    protected:
        // Internal simulation object (allocated on build)
        sim::SoftBody* soft_body = nullptr;

        // Backup save for reset
        PackedVector2Array backed_ploygon;

        // Editor-facing parameters
        int unit = 5;
        double mass = 1.0;
        double particles_radius = 1.0;
        double friction = 0.5;
        double restitution = 0.5;
        double stiffness = 0.8;
        double damping = 0.1;

        static void _bind_methods();

    public:
        GDSoftBody_2() {}
        ~GDSoftBody_2();

        void build();
        void update(std::vector<sim::Particle*> new_border);
        void reset();

        // getters / setters
        void set_unit(const int u) { unit = u; }
        int get_unit() const { return unit; }

        void set_mass(const double m) { mass = m; }
        double get_mass() const { return mass; }
        void set_radius(const double r) { particles_radius = r; }
        double get_radius() const { return particles_radius; }
        
        void set_friction(double f) { friction = f; }
        double get_friction() const { return friction; }
        void set_restitution(double r) { restitution = r; }
        double get_restitution() const { return restitution; }

        void set_stiffness(const double s) { stiffness = s; }
        double get_stiffness() const { return stiffness; }
        void set_damping(const double d) { damping = d; }
        double get_damping() const { return damping; }

        // Access to the sim object
        sim::SoftBody* get_sim_softbody() const { return soft_body; }
        sim::SoftBody* take_sim_softbody() {
            sim::SoftBody* sb = soft_body;
            soft_body = nullptr;
            return sb;
        }
    };
}
