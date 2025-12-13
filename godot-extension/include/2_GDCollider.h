//2_GDCollider.h
#pragma once
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/binder_common.hpp>

#include "PlaneWorldCollider.h"
#include "CircleWorldCollider.h"

#include "GDParticle.h"
#include "GDConstraint.h"

namespace godot {

    class GDCollider : public Node2D {
        GDCLASS(GDCollider, Node2D)

    protected:
        // Internal simulation object (allocated on build)
        sim::WorldCollider* world_collider = nullptr;

        enum COLLIDER_TYPE{
            PLANE,
            INNERCIRCLE,
            OUTERCIRCLE,
        };

        // Editor-facing parameters
        Vector2 point = Vector2();
        double distance = 1.;
        int collider_type = PLANE;

        double friction = 0.5;
        double restitution = 0.5;
        
        static void _bind_methods();

    public:
        GDCollider() {}
        ~GDCollider();

        void build();
        void reset();
        void _draw() override;

        // getters / setters
        void set_point(const Vector2& p) { point = p; }
        Vector2 get_point() const { return point; }
        void set_distance(const double& d) { distance = d; }
        double get_distance() const { return distance; }
        void set_collider(int ct) { collider_type = (COLLIDER_TYPE)ct; }
        int get_collider() const { return (int)collider_type; }


        void set_friction(double f) { friction = f; }
        double get_friction() const { return friction; }
        void set_restitution(double r) { restitution = r; }
        double get_restitution() const { return restitution; }

        // Access to the sim object
        sim::WorldCollider* get_sim_collider() const { return world_collider; }
        sim::WorldCollider* take_sim_collider() {
            sim::WorldCollider* wc = world_collider;
            world_collider = nullptr;
            return wc;
        }
    };
}
