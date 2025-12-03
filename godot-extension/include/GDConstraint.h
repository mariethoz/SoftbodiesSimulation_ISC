//GDConstraint.h
#pragma once
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include "Constraint.h"
#include "GDParticle.h"

namespace godot {

    class GDConstraint : public Resource {
        GDCLASS(GDConstraint, Resource)

    private:
        // Editor-facing parameters
        Ref<GDParticle> part1;
        Ref<GDParticle> part2;
        double stiffness = 0.8;
        double damping = 0.1;

        // Internal simulation object (allocated on build)
        sim::Constraint* constraint = nullptr;

    protected:
        static void _bind_methods();

    public:
        GDConstraint() {}
        ~GDConstraint();

        void build();
        void reset();

        // setters/getters
        void set_part1(const Ref<GDParticle> &p) { part1 = p; }
        Ref<GDParticle> get_part1() const { return part1; }

        void set_part2(const Ref<GDParticle> &p) { part2 = p; }
        Ref<GDParticle> get_part2() const { return part2; }

        void set_stiffness(double s) { stiffness = s; }
        double get_stiffness() const { return stiffness; }

        void set_damping(double d) { damping = d; }
        double get_damping() const { return damping; }

        // Access to the sim object
        sim::Constraint* get_sim_constraint() const { return constraint; }
    };

}
