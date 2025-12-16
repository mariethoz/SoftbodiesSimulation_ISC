//2_GDCollider.h
#pragma once
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/binder_common.hpp>

#include "PlaneWorldCollider.h"
#include "CircleWorldCollider.h"

#include "GDParticle.h"
#include "GDConstraint.h"

namespace godot {
    /**
     * @brief A Node2D representing a collider in the soft body simulation
     * 
     * This node allows to define static colliders in the 2D world that interact
     * with soft bodies. It supports different collider shapes and physical
     * properties.
     */
    class GDCollider : public Node2D {
        GDCLASS(GDCollider, Node2D)

    protected:
        // Internal simulation object (allocated on build)
        sim::WorldCollider* world_collider = nullptr;   /// Pointer to the simulation collider

        /// @brief Types of colliders supported
        enum COLLIDER_TYPE{
            PLANE,          /// Infinite plane collider
            INNERCIRCLE,    /// Disc-container collider with collision from inside
            OUTERCIRCLE,    /// Disc-like collider with collision from outside
        };

        // Editor-facing parameters
        Vector2 point = Vector2(0,1);   /// A point on the collider (e.g., a point on the plane or center of circle)
        double distance = 1.;           /// Distance parameter (e.g., plane normal distance or circle radius)
        int collider_type = PLANE;      /// Type of the collider

        double friction = 0.5;      /// Friction coefficient [smooth 0 < 1 rough]
        double restitution = 0.5;   /// Restitution (bounciness) coefficient [sticky 0 < 1 reflect]
        
        static void _bind_methods();

    public:
        GDCollider() {}
        ~GDCollider();

        // core function
        /** 
         * @brief Build the internal simulation collider based on the current parameters
         * 
         * This function creates the appropriate WorldCollider instance
         * according to the selected collider type and parameters.
         */
        void build();
        
        /** 
         * @brief Reset the collider to its default state
         * 
         * This function clears the internal simulation collider and resets
         * parameters to their defaults.
         */
        void reset();

        /** 
         * @brief Draw the collider in the editor for visualization
         * 
         * This function overrides the Node2D _draw method to render
         * the collider shape in the editor viewport.
         */
        void _draw() override;

        // getters / setters
        void set_point(const Vector2& p) { point = p; }
        Vector2 get_point() const { return point; }
        void set_distance(const double& d) { distance = -d; }
        double get_distance() const { return -distance; }
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
