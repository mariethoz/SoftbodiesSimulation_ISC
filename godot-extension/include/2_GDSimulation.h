//GDSimulation.h
#pragma once
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/core/binder_common.hpp>

#include <vector>
#include <map>

#include "Simulation.h"
#include "2_GDSoftBody.h"
#include "SoftBody.h"

namespace godot {
    class GDSimulation_2 : public Node2D {
        GDCLASS(GDSimulation_2, Node2D);
    
    private:
        sim::Simulation simulation;
        Vector2 gravity = Vector2(0,-10);
        bool _verbose = false;

        std::map<sim::SoftBody*,GDSoftBody_2*> bodies;

        void step_simulation(double delta) { simulation.step(delta); }
        void draw_simulation();

    protected:
        static void _bind_methods();
        void build();

    public:
        GDSimulation_2() {}
        ~GDSimulation_2() { simulation.clear(); }

        
        // core function
        void reset_simulation() { build(); }

        // getters / setters
        void set_gravity(const Vector2 g) { gravity = g; }
        Vector2 get_gravity() const { return gravity; }

        void set_debug(const bool d) { _verbose = d; }
        bool get_debug() const { return _verbose; }

        // Godot function
        void _ready() override {
            if (Engine::get_singleton()->is_editor_hint()) {
                // Running inside the editor → not ready to simulate
                return;
            }
            build();
        }

        void _process(double delta) override {
            if (Engine::get_singleton()->is_editor_hint()) {
                // Running inside the editor → skip gameplay logic
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

