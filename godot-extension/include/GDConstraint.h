//GDConstraint.h
#pragma once
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include "Constraint.h"
#include "GDParticle.h"

namespace godot {
    /**
     * @warning This file is deprecated and will be removed in future versions. It will not be replaced.
     * Please use the new GDSoftBody_2 classes instead.
     *
     * @brief A Resource representing a constraint between two particles in the simulation
     * 
     * This resource defines a constraint that connects two particles,
     * controlling their relative motion based on stiffness and damping parameters.
     */
    class GDConstraint : public Resource {
        GDCLASS(GDConstraint, Resource)

    private:
        // Internal simulation object (allocated on build)
        sim::Constraint* constraint = nullptr;  /// Pointer to the simulation constraint

        // Editor-facing parameters
        Ref<GDParticle> part1;                  /// First particle of the constraint
        Ref<GDParticle> part2;                  /// Second particle of the constraint
        double stiffness = 0.8;                 /// Stiffness of the constraint [flexible 0 < 1 rigid]
        double damping = 0.1;                   /// Damping factor for oscillations [oscilling 0 < 1 freezing]

    protected:
        static void _bind_methods();

    public:
        GDConstraint() {}
        ~GDConstraint();

        // core function
        /** 
         * @brief Build the internal simulation constraint based on the current parameters
         * 
         * This function creates the Constraint instance connecting the two
         * specified particles with the defined stiffness and damping.
         */
        void build();
        /** 
         * @brief Reset the constraint to its default state
         * 
         * This function clears the internal simulation constraint.
         */
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
        sim::Constraint* take_sim_constraint() {
            sim::Constraint* c = constraint;
            constraint = nullptr;
            return c;
        }
    };

}
