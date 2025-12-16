//GDSimulation.h
#pragma once
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/core/binder_common.hpp>

#include <vector>
#include <map>

#include "2_GDSoftBody.h"
#include "2_GDCollider.h"
#include "Simulation.h"
#include "SoftBody.h"

namespace godot {
    /**
     * @brief A Node2D representing the soft body simulation
     * 
     * This node manages the soft body simulation, including bodies,
     * colliders, and simulation parameters such as gravity.
     */
    class GDSimulation_2 : public Node2D {
        GDCLASS(GDSimulation_2, Node2D);
    
    private:
        sim::Simulation simulation;                     /// Internal simulation object
        std::map<sim::SoftBody*,GDSoftBody_2*> bodies;  /// Mapping of simulation bodies to Godot nodes
        std::vector<GDCollider*> colliders;              /// List of colliders in the simulation

        // Editor-facing parameters
        Vector2 gravity = Vector2(0,10);               /// Gravity vector
        bool _verbose = false;                         /// Verbose debug drawing

        // Helper
        void step_simulation(double delta) { simulation.step(delta); }
        /**
         * @brief Draw the simulation state for debugging purposes
         * This function visualizes the soft bodies and their particles
         * within the Godot editor.
         */
        void draw_simulation();

    protected:
        static void _bind_methods();
        /**
         * @brief Build or rebuild the simulation based on current parameters
         * 
         * This function initializes the simulation, adding bodies and
         * colliders as defined in the scene.
         */
        void build();

    public:
        GDSimulation_2() {}
        ~GDSimulation_2() { simulation.clear(); }

        // core function
        /** 
         * @brief Reset the simulation to its initial state
         * 
         * This function clears and rebuilds the simulation.
         */
        void reset_simulation() { build(); }

        // getters / setters
        void set_gravity(const Vector2 g) { gravity = g; }
        Vector2 get_gravity() const { return gravity; }

        void set_debug(const bool d) { _verbose = d; }
        bool get_debug() const { return _verbose; }

        // Godot function
        void _ready() override {
            if (Engine::get_singleton()->is_editor_hint()) {
                // Running inside the editor -> not ready to simulate
                return;
            }
            build();
        }

        void _process(double delta) override {
            if (Engine::get_singleton()->is_editor_hint()) {
                // Running inside the editor -> skip gameplay logic
                queue_redraw();
                return;
            }
            step_simulation(delta);
            queue_redraw();
        }

        void _draw() override {
            if (Engine::get_singleton()->is_editor_hint()) return;
            draw_simulation();
        }
    };
}

