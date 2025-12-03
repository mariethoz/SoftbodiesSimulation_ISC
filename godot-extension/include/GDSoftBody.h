//SoftBody.h
#pragma once
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include "SoftBody.h"
#include "GDParticle.h"
#include "GDConstraint.h"

namespace godot {

    class GDSoftBody : public Resource {
        GDCLASS(GDSoftBody, Resource)

    private:
        // Editor-facing parameters
        Array particles;
        Array constraints;
        double friction = 0.5;
        double restitution = 0.5;

        // Internal simulation object (allocated on build)
        sim::SoftBody* soft_body = nullptr;

    protected:
        static void _bind_methods();

    public:
        GDSoftBody() {}
        ~GDSoftBody();

        void build();
        void reset();

        // getters / setters
        void set_particles(const Array &a) { particles = a; }
        Array get_particles() const { return particles; }

        void set_constraints(const Array &a) { constraints = a; }
        Array get_constraints() const { return constraints; }

        void set_friction(double f) { friction = f; }
        double get_friction() const { return friction; }

        void set_restitution(double r) { restitution = r; }
        double get_restitution() const { return restitution; }

        // Access to the sim object
        sim::SoftBody* get_sim_softbody() const { return soft_body; }
    };

}
