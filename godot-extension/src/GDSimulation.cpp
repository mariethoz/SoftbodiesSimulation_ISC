#include "GDSimulation.h"
#include "CircleWorldCollider.h"
#include "PlaneWorldCollider.h"
#include "GDVector2.h"

using namespace godot;
using namespace convert;

const int SCALE_DRAW = 20;

void GDParticleSimulation::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_time_step", "step"), &GDParticleSimulation::set_time_step);
    ClassDB::bind_method(D_METHOD("get_time_step"), &GDParticleSimulation::get_time_step);

    ClassDB::bind_method(D_METHOD("set_particles", "particles"), &GDParticleSimulation::set_particles);
    ClassDB::bind_method(D_METHOD("get_particles"), &GDParticleSimulation::get_particles);

    ClassDB::bind_method(D_METHOD("reset_simulation"), &GDParticleSimulation::reset_simulation);

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "time_step"), "set_time_step", "get_time_step");
    ADD_PROPERTY(PropertyInfo(Variant::PACKED_VECTOR2_ARRAY, "particles"), "set_particles", "get_particles");
}

GDParticleSimulation::GDParticleSimulation() {
}

GDParticleSimulation::~GDParticleSimulation() {
}

void GDParticleSimulation::reset_simulation() {
    simulation.clear();
    _ready();
}

void GDParticleSimulation::_ready() {

    // Same setup as your main.cpp
    simulation.setGravity(sim::Vector2(0, -10 / step));
    simulation.addCollider(new sim::PlaneCollider(sim::Vector2(0,1), -10.0f));
    simulation.addCollider(new sim::OuterCircleCollider(sim::Vector2(0,-10), 5.0f));
    simulation.addCollider(new sim::InnerCircleCollider(sim::Vector2(0,0), 15.0f));

    // Particles
    if (particles.size() == 0) {
        std::vector<sim::Vector2> positions = {
            {  2.0f, -1.1f },
            {  2.0f,  1.1f },
            { -2.0f,  1.1f },
            {  0.0f, -2.0f },
            {  1.5f,  2.0f },
            { -3.0f,  0.5f },
            {  4.0f,  1.0f },
            { -1.5f, -1.5f },
            {  0.0f,  4.5f },
            {  2.5f, -3.0f }
        };

        for (auto &pos : positions) {
            std::vector<sim::Particle> p;
            p.emplace_back(pos, 1.0f);
            sim::SoftBody body(p, 0.5f, 0.5f);
            simulation.addBody(body);
        }
    } else {
        for (auto &part : particles) {
            std::vector<sim::Particle> p;
            p.emplace_back(convert::from_godot(part), 1.0f);
            sim::SoftBody body(p, 0.5f, 0.5f);
            simulation.addBody(body);
        }
    }
}

void GDParticleSimulation::_process(double delta) {
    simulation.step(step);
    queue_redraw();
}

void GDParticleSimulation::_draw() {
    for (auto &body : simulation.getBodies()) {
        for (auto &p : body.getParticles()) {
            Vector2 pos = convert::to_godot(p.getPosition()) * SCALE_DRAW;
            float radius = p.getRadius() * SCALE_DRAW;
            draw_circle(pos, radius, Color(1,1,1));
        }
    }
    for (auto* col: simulation.getColliders()) {
        if (auto* plane = dynamic_cast<sim::PlaneCollider*>(col)) {
            Vector2 n = to_godot(plane->getNormal());
            Vector2 origin = Vector2(0, -plane->getDistance()) * 20;

            draw_line(origin - n.rotated(Math_PI/2) * 500,
                    origin + n.rotated(Math_PI/2) * 500,
                    Color(1,0,0), 2);
        }
        if (auto* c = dynamic_cast<sim::CircleCollider*>(col)) {
            Vector2 center = convert::to_godot(c->getCenter()) * SCALE_DRAW;
            float radius = c->getRadius() * SCALE_DRAW;

            draw_arc(center, radius, 0, Math_TAU, 64, Color(0,1,0), 2);
        }
    }
}
