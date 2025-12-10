//GDSimulation.cpp
#include "GDSimulation.h"
#include "GDVector2.h"
#include "CircleWorldCollider.h"
#include "PlaneWorldCollider.h"
#include "Particle.h"
#include "SoftBody.h"

using namespace godot;
using namespace convert;

const int SCALE_DRAW = 5;


void GDSimulation::_bind_methods() {
    ClassDB::bind_method(D_METHOD("reset_simulation"), &GDSimulation::reset_simulation);
}

void GDSimulation::draw_simulation() {
    for (auto body : simulation.getBodies()) {
        for (auto p : body->getParticles()) {
            Vector2 pos = convert::to_godot(p->getPosition()) * SCALE_DRAW;
            float radius = p->getRadius() * SCALE_DRAW;
            draw_circle(pos, radius, Color(1,0,0));
        }
        for (auto c: body->getConstraints()) {
            Vector2 p1 = convert::to_godot(c->getPart1()) * SCALE_DRAW;
            Vector2 p2 = convert::to_godot(c->getPart2()) * SCALE_DRAW;
            draw_line(p1,p2,Color(1,1,1), 3.0);
        }
    }
    for (auto* col: simulation.getColliders()) {
        if (auto* plane = dynamic_cast<sim::PlaneCollider*>(col)) {
            Vector2 n = to_godot(plane->getNormal());
            Vector2 origin = Vector2(0, -plane->getDistance()) * SCALE_DRAW;

            draw_line(origin - n.rotated(Math_PI/2) * 500,
                    origin + n.rotated(Math_PI/2) * 500,
                    Color(0,0,1), 2);
        }
        if (auto* c = dynamic_cast<sim::CircleCollider*>(col)) {
            Vector2 center = convert::to_godot(c->getCenter()) * SCALE_DRAW;
            float radius = c->getRadius() * SCALE_DRAW;

            draw_arc(center, radius, 0, Math_TAU, 64, Color(0,0,1), 2);
        }
    }
}

void GDParticleSimulation::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_particles", "particles"), &GDParticleSimulation::set_particles);
    ClassDB::bind_method(D_METHOD("get_particles"), &GDParticleSimulation::get_particles);

    ADD_PROPERTY(PropertyInfo(Variant::PACKED_VECTOR2_ARRAY, "particles"), "set_particles", "get_particles");
}

void GDParticleSimulation::build() {
    simulation.clear();
    simulation.setGravity(sim::Vector2(0, -10));
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

        // Create one SoftBody per particle
        for (auto& pos : positions) {
            std::vector<sim::Particle*> particles;
            particles.push_back(new sim::Particle(pos, 1.0f));  // allocate particles
            simulation.addBody(new sim::SoftBody(particles));
        }
    } else {
        for (auto &part : particles) {
            std::vector<sim::Particle*> p;
            p.emplace_back(new sim::Particle(convert::from_godot(part), 1.0f));
            simulation.addBody(new sim::SoftBody(p));
        }
    }
}

void GDParticleSimulation::_ready() {
    build();
}

void GDSoftBodySimulation::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_bodies", "bodies"), &GDSoftBodySimulation::set_bodies);
    ClassDB::bind_method(D_METHOD("get_bodies"), &GDSoftBodySimulation::get_bodies);

    ADD_PROPERTY(
        PropertyInfo(Variant::ARRAY, "bodies", PROPERTY_HINT_ARRAY_TYPE,
            String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":GDSoftBody"
        ),
        "set_bodies","get_bodies"
    );
}

void GDSoftBodySimulation::build() {
    simulation.clear();
    simulation.setGravity(sim::Vector2(0, -10));
    simulation.addCollider(new sim::PlaneCollider(sim::Vector2(0,1), -10.0f));
    simulation.addCollider(new sim::OuterCircleCollider(sim::Vector2(0,-10), 5.0f));
    simulation.addCollider(new sim::InnerCircleCollider(sim::Vector2(0,0), 15.0f));

    // Load GDSoftBody resources into the simulation
    for (int i = 0; i < bodies.size(); i++) {
        Ref<GDSoftBody> sb = bodies[i];
        if (!sb.is_valid())
            continue;

        sb->reset();  // clear the previous sim::SoftBody*
        sb->build();  // creates sim::SoftBody* internally

        if (sb->get_sim_softbody()){
            simulation.addBody(sb->take_sim_softbody());
        }
    }
}

void GDSoftBodySimulation::_ready() {
    build();
}

void GDSoftBodySimulationBtn::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_bodies", "bodies"), &GDSoftBodySimulationBtn::set_bodies);
    ClassDB::bind_method(D_METHOD("get_bodies"), &GDSoftBodySimulationBtn::get_bodies);
    ClassDB::bind_method(D_METHOD("add_body", "body"), &GDSoftBodySimulationBtn::add_body);

    ADD_PROPERTY(
        PropertyInfo(Variant::ARRAY, "bodies", PROPERTY_HINT_ARRAY_TYPE,
            String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":GDSoftBody"
        ),
        "set_bodies","get_bodies"
    );
}

void GDSoftBodySimulationBtn::build() {
    simulation.clear();
    simulation.setGravity(sim::Vector2(0, -10));
    simulation.addCollider(new sim::PlaneCollider(sim::Vector2(0,1), -100.0f));
    simulation.addCollider(new sim::OuterCircleCollider(sim::Vector2(0,-100), 50.0f));
    simulation.addCollider(new sim::InnerCircleCollider(sim::Vector2(0,0), 150.0f));

    // Load GDSoftBody resources into the simulation
    for (int i = 0; i < bodies.size(); i++) {
        Ref<GDSoftBody> sb = bodies[i];
        if (!sb.is_valid())
            continue;

        sb->reset();  // clear the previous sim::SoftBody*
        sb->build();  // creates sim::SoftBody* internally

        if (sb->get_sim_softbody()){
            simulation.addBody(sb->take_sim_softbody());
        }
    }
}

void GDSoftBodySimulationBtn::add_body(Ref<GDSoftBodyPolygone> body) {
    body->reset();
    body->build();
    if (body->get_sim_softbody())
        simulation.addBody(body->take_sim_softbody());
}

void GDSoftBodySimulationBtn::_ready()
{
    build();
}
