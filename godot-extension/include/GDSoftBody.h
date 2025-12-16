//SoftBody.h
#pragma once
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include "SoftBody.h"
#include "GDParticle.h"
#include "GDConstraint.h"

namespace godot {
    /**
     * @warning This file is deprecated and will be removed in future versions. It will not be replaced.
     * Please use the new GDSoftBody_2 classes instead.
     * 
     * @brief A Resource representing a soft body in the simulation
     * 
     * This resource defines a soft body composed of particles and constraints,
     * along with physical properties such as friction, restitution, stiffness,
     * and damping.
     */

    class GDSoftBody : public Resource {
        GDCLASS(GDSoftBody, Resource)

    protected:
        // Internal simulation object (allocated on build)
        sim::SoftBody* soft_body = nullptr; /// Pointer to the simulation soft body
        // Editor-facing parameters
        Array particles;                    /// Array of GDParticle resources
        Array constraints;                  /// Array of GDConstraint resources
        double friction = 0.1;              /// Friction coefficient of the soft body
        double restitution = 0.9;           /// Restitution (bounciness) coefficient of the soft body
        double stiffness = 0.8;             /// Stiffness of the constraints
        double damping = 0.1;               /// Damping factor for the constraints

        static void _bind_methods();

    public:
        GDSoftBody() {}
        ~GDSoftBody();

        // core function
        virtual void build();
        void reset();

        // getters / setters
        void set_friction(double f) { friction = f; }
        double get_friction() const { return friction; }
        void set_restitution(double r) { restitution = r; }
        double get_restitution() const { return restitution; }
        
        void set_Stiffness(double s) { stiffness = s; }
        double get_stiffness() const { return stiffness; }
        void set_damping(double d) { damping = d; }
        double get_damping() const { return damping; }

        // Access to the sim object
        sim::SoftBody* get_sim_softbody() const { return soft_body; }
        sim::SoftBody* take_sim_softbody() {
            sim::SoftBody* sb = soft_body;
            soft_body = nullptr;
            return sb;
        }
    };

    /**
     * @brief GDSoftBodyCustom is a GDSoftBody that allows custom definition of particles and constraints.
     * 
     * It enables users to define their own particles and constraints for the soft body.
     */
    class GDSoftBodyCustom : public GDSoftBody {
        GDCLASS(GDSoftBodyCustom, GDSoftBody)
    
    protected:
        static void _bind_methods();

    public:
        // getters / setters
        void set_particles(const Array &a) { particles = a; }
        Array get_particles() const { return particles; }

        void set_constraints(const Array &a) { constraints = a; }
        Array get_constraints() const { return constraints; }
    };

    /**
     * @brief GDSoftBodyTriange is a GDSoftBody that represents a triangular soft body.
     * 
     * It allows users to define a soft body in the shape of a triangle by specifying its three corners.
     */
    class GDSoftBodyTriange : public GDSoftBody {
        GDCLASS(GDSoftBodyTriange, GDSoftBody)
    
    private:
        // Editor-facing parameters
        Vector2 corner_1;
        Vector2 corner_2;
        Vector2 corner_3;

    protected:
        static void _bind_methods();

    public:
        void build() override;

        // getters / setters
        void set_corner_1(const Vector2 c) { corner_1 = c; }
        Vector2 get_corner_1() const { return corner_1; }
        void set_corner_2(const Vector2 c) { corner_2 = c; }
        Vector2 get_corner_2() const { return corner_2; }
        void set_corner_3(const Vector2 c) { corner_3 = c; }
        Vector2 get_corner_3() const { return corner_3; }
    };

    /**
     * @brief GDSoftBodySquare is a GDSoftBody that represents a square soft body.
     * 
     * It allows users to define a soft body in the shape of a square by specifying its four corners.
     */
    class GDSoftBodySquare : public GDSoftBody {
        GDCLASS(GDSoftBodySquare, GDSoftBody)
    
    private:
        // Editor-facing parameters
        Vector2 corner_1;
        Vector2 corner_2;
        Vector2 corner_3;
        Vector2 corner_4;

    protected:
        static void _bind_methods();

    public:
        void build() override;

        // getters / setters
        void set_corner_1(const Vector2 c) { corner_1 = c; }
        Vector2 get_corner_1() const { return corner_1; }
        void set_corner_2(const Vector2 c) { corner_2 = c; }
        Vector2 get_corner_2() const { return corner_2; }
        void set_corner_3(const Vector2 c) { corner_3 = c; }
        Vector2 get_corner_3() const { return corner_3; }
        void set_corner_4(const Vector2 c) { corner_4 = c; }
        Vector2 get_corner_4() const { return corner_4; }
    };

    /**
     * @brief GDSoftBodyPolygone is a GDSoftBody that represents a polygonal soft body.
     * 
     * It allows users to define a soft body in the shape of a polygon by specifying its border points.
     */
    class GDSoftBodyPolygone : public GDSoftBody {
        GDCLASS(GDSoftBodyPolygone, GDSoftBody)
    
    private:
        PackedVector2Array border;      /// Array of border points defining the polygon
        int unit = 10;                  /// Unit size for particle placement
        double particles_radius = 1.;   /// Radius of the particles
        double mass = 1.;               /// Mass of each particle
        double stiffness = 0.8;         /// Stiffness of the constraints
        double damping = 0.1;           /// Damping factor for the constraints

    protected:
        static void _bind_methods();

    public:
        void build() override;

        // getters / setters
        void set_border(const Array &b) { border = b; }
        Array get_border() const { return border; }
        void set_unit(const double u) { unit = u; }
        double get_unit() const { return unit; }
        void set_radius(const double r) { particles_radius = r; }
        double get_radius() const { return particles_radius; }
        void set_mass(const double m) { mass = m; }
        double get_mass() const { return mass; }
        void set_stiffness(const double s) { stiffness = s; }
        double get_stiffness() const { return stiffness; }
        void set_damping(const double d) { damping = d; }
        double get_damping() const { return damping; }
    };
}
