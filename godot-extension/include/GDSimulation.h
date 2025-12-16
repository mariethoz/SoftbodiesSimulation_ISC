// GDSimulation.h
#pragma once
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/core/binder_common.hpp>

#include "CircleWorldCollider.h"
#include "PlaneWorldCollider.h"

#include "Simulation.h"
#include "SoftBody.h"

#include "GDSoftBody.h"

namespace godot {
    /**
     * @warning This file is deprecated and will be removed in future versions. It will not be replaced.
     * Please use the new GDSimulation_2 classes instead.
     */
    /**
     * @brief GDSimulation is the Godot wrapper for the Simulation class.
     * 
     * It extends Node2D to allow for 2D positioning within the Godot scene.
     * This class manages the simulation lifecycle, including stepping the simulation
     * and rendering the simulation state.
     */
    class GDSimulation : public Node2D {
        GDCLASS(GDSimulation, Node2D);
    
    private:
        void step_simulation(double delta) { simulation.step(delta); }
        void draw_simulation();

    protected:
        sim::Simulation simulation; /// Internal simulation object
        static void _bind_methods();
        virtual void build() = 0;

    public:
        GDSimulation() {}
        ~GDSimulation() { simulation.clear(); }

        // core function
        /**
         * @brief Reset the simulation to its initial state
         */
        void reset_simulation() { build(); }

        // Godot function
        virtual void _ready() override = 0;
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
            draw_simulation();
        }
    };

    /**
     * @brief GDParticleSimulation is a GDSimulation that simulates particles.
     * 
     * It manages a collection of particles and updates their state each frame.
     */
    class GDParticleSimulation : public GDSimulation {
        GDCLASS(GDParticleSimulation, GDSimulation);

    private:
        PackedVector2Array particles;

    protected:
        static void _bind_methods();
        void build();

    public:
        GDParticleSimulation() {}
        ~GDParticleSimulation() {}

        void set_particles(const PackedVector2Array &p_particles) { particles = p_particles; }
        PackedVector2Array get_particles() const { return particles; }

        void _ready() override;
    };
    
    /**
     * @brief GDSoftBodySimulation is a GDSimulation that simulates soft bodies.
     * 
     * It manages a collection of soft bodies and updates their state each frame.
     */
    class GDSoftBodySimulation : public GDSimulation {
        GDCLASS(GDSoftBodySimulation, GDSimulation);

    private:
        Array bodies;

    protected:
        static void _bind_methods();
        void build();

    public:
        GDSoftBodySimulation() {}
        ~GDSoftBodySimulation() {}

        void set_bodies(const Array &p_bodies) { bodies = p_bodies; }
        Array get_bodies() const { return bodies; }

        void _ready() override;
    };

    /**
     * @brief GDSoftBodySimulationBtn is a GDSimulation that simulates soft bodies defined as polygons.
     * 
     * It manages a collection of GDSoftBodyPolygone bodies and updates their state each frame.
     */
    class GDSoftBodySimulationBtn : public GDSimulation {
        GDCLASS(GDSoftBodySimulationBtn, GDSimulation);

    private:
        Array bodies;

    protected:
        static void _bind_methods();
        void build();

    public:
        GDSoftBodySimulationBtn() {}
        ~GDSoftBodySimulationBtn() {}

        void set_bodies(const Array &p_bodies) { bodies = p_bodies; }
        Array get_bodies() const { return bodies; }

        void add_body(Ref<GDSoftBodyPolygone> body);

        void _ready() override;
    };
}