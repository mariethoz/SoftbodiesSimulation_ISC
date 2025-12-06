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
        // Internal simulation object (allocated on build)
        sim::SoftBody* soft_body = nullptr;

    protected:
        // Editor-facing parameters
        Array particles;
        Array constraints;
        double friction = 0.5;
        double restitution = 0.5;
        double stiffness = 0.8;
        double damping = 0.1;

        static void _bind_methods();

    public:
        GDSoftBody() {}
        ~GDSoftBody();

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

}
