//GDSimulation.h
#pragma once
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/core/binder_common.hpp>

#include "CircleWorldCollider.h"
#include "PlaneWorldCollider.h"

#include "Simulation.h"

#include "GDSoftBody.h"

namespace godot {
    class GDSimulation : public Node2D {
        GDCLASS(GDSimulation, Node2D);
    
    private:
        void step_simulation(double delta) { simulation.step(delta); }
        void draw_simulation();

    protected:
        sim::Simulation simulation;
        static void _bind_methods();
        virtual void build() = 0;

    public:
        GDSimulation() {}
        ~GDSimulation() { simulation.clear(); }

        virtual void _ready() override = 0;
        void reset_simulation() { build(); }

        void _process(double delta) override {
            step_simulation(delta);
            queue_redraw();
        }

        void _draw() override {
            draw_simulation();
        }
    };

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
}