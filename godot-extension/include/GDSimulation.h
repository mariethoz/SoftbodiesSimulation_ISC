#pragma once
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include "CircleWorldCollider.h"
#include "PlaneWorldCollider.h"

#include "Simulation.h"

namespace godot {

    class GDParticleSimulation : public Node2D {
        GDCLASS(GDParticleSimulation, Node2D);

    private:
        sim::Simulation simulation;
        PackedVector2Array particles;

    protected:
        static void _bind_methods();

    public:
        GDParticleSimulation();
        ~GDParticleSimulation();

        void set_particles(const PackedVector2Array &p_particles) { particles = p_particles; }
        PackedVector2Array get_particles() const { return particles; }
        void reset_simulation();

        void _ready() override;
        void _process(double delta) override;
        void _draw() override;
    };

}