//2_GDSoftBody.h
#pragma once
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/polygon2d.hpp>
#include <godot_cpp/core/binder_common.hpp>

#include "SoftBody.h"

#include "GDParticle.h"
#include "GDConstraint.h"

namespace godot {
    /**
     * @brief A Polygon2D representing a soft body in the simulation
     * 
     * This node allows to define a soft body using a polygon shape.
     * It manages the particles and constraints that make up the soft body
     * and provides parameters to control its physical behavior.
     */
    class GDSoftBody_2 : public Polygon2D {
        GDCLASS(GDSoftBody_2, Polygon2D)

    protected:
        // Internal simulation object (allocated on build)
        sim::SoftBody* soft_body = nullptr; /// Pointer to the simulation soft body

        // Backup save for reset
        PackedVector2Array backed_ploygon;  /// Backup of the original polygon points

        // Editor-facing parameters
        int unit = 5;                       /// Distance between particles in the mesh
        double mass = 1.0;                  /// Mass of each particle
        double particles_radius = 1.0;      /// Radius of each particle
        double friction = 0.5;              /// Friction coefficient [smooth 0 < 1 rough]
        double restitution = 0.5;           /// Restitution (bounciness) coefficient [sticky 0 < 1 reflect]
        double stiffness = 0.8;             /// Stiffness of the constraints [flexible 0 < 1 rigid]
        double damping = 0.1;               /// Damping factor of the constraints [oscilling 0 < 1 freezing]

        static void _bind_methods();

    public:
        GDSoftBody_2() {}
        ~GDSoftBody_2();

        // core function
        /** 
         * @brief Build the internal simulation soft body based on the current polygon and parameters
         * 
         * This function creates the SoftBody instance by meshing the polygon
         * into particles and constraints according to the specified parameters.
         */
        void build();
        
        /** 
         * @brief Update the polygon2D shape based on new border particles
         * 
         * This function modifies the polygon points to match the positions
         * of the simulation current border particles.
         */
        void update(std::vector<sim::Particle*> new_border);

        /** 
         * @brief Reset the soft body to its initial state
         * 
         * This function restores the polygon to its original shape
         * and rebuilds the internal simulation soft body.
         */
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
