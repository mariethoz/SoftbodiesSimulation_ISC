//GDParticle.h
#pragma once
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include "Particle.h"
#include "GDVector2.h"

namespace godot {
    /**
     * @warning This file is deprecated and will be removed in future versions. It will not be replaced.
     * Please use the new GDSoftBody_2 classes instead.
     * 
     * @brief A Resource representing a particle in the soft body simulation
     * 
     * This resource defines a particle with position, mass, radius,
     * and pinning properties for use in soft body simulations.
     */
    class GDParticle : public Resource {
        GDCLASS(GDParticle, Resource)

    private:
        // Internal simulation object (allocated on build)
        sim::Particle* particle = nullptr;  /// Pointer to the simulation particle

        // Editor-facing parameters
        Vector2 position_gd;                /// Position of the particle
        double mass = 1.0;                  /// Mass of the particle
        double radius = 1.0;                /// Radius of the particle
        bool pinned = false;                /// Whether the particle is pinned (immovable)

    protected:
        static void _bind_methods();

    public:
        GDParticle() {}
        ~GDParticle();

        // core function
        /** 
         * @brief Build the internal simulation particle based on the current parameters
         * 
         * This function creates the Particle instance with the defined
         * position, mass, radius, and pinning state.
         */
        void build();
        
        /** 
         * @brief Reset the particle to its default state
         * 
         * This function clears the internal simulation particle.
         */
        void reset();

        // getters / setters
        void set_position(const Vector2 &p) { position_gd = p; }
        Vector2 get_position() const { return position_gd; }
        void set_mass(double m) { mass = m; }
        double get_mass() const { return mass; }
        void set_radius(double r) { radius = r; }
        double get_radius() const { return radius; }
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