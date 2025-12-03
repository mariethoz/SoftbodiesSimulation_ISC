#include "GDSoftBody.h"
#include "GDVector2.h"
using namespace godot;

void GDSoftBody::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_particles","particles"), &GDSoftBody::set_particles);
    ClassDB::bind_method(D_METHOD("get_particles"), &GDSoftBody::get_particles);

    ClassDB::bind_method(D_METHOD("set_constraints","constraints"), &GDSoftBody::set_constraints);
    ClassDB::bind_method(D_METHOD("get_constraints"), &GDSoftBody::get_constraints);

    ClassDB::bind_method(D_METHOD("set_friction","friction"), &GDSoftBody::set_friction);
    ClassDB::bind_method(D_METHOD("get_friction"), &GDSoftBody::get_friction);

    ClassDB::bind_method(D_METHOD("set_restitution","restitution"), &GDSoftBody::set_restitution);
    ClassDB::bind_method(D_METHOD("get_restitution"), &GDSoftBody::get_restitution);

    ClassDB::bind_method(D_METHOD("build"), &GDSoftBody::build);

    ADD_PROPERTY(
        PropertyInfo(Variant::ARRAY, "particles", PROPERTY_HINT_ARRAY_TYPE,
        String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":GDParticle"),
        "set_particles", "get_particles"
    );
    ADD_PROPERTY(
        PropertyInfo(Variant::ARRAY, "constraints", PROPERTY_HINT_ARRAY_TYPE,
        String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":GDConstraint"),
        "set_constraints", "get_constraints"
    );
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "friction"), "set_friction", "get_friction");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "restitution"), "set_restitution", "get_restitution");
}

GDSoftBody::~GDSoftBody() {
    if (soft_body) {
        delete soft_body;
        soft_body = nullptr;
    }
}

void GDSoftBody::build() {
    if (soft_body) return;

    // Build the particles
    std::vector<sim::Particle*> sim_particles;
    sim_particles.reserve(particles.size());
    for (int i = 0; i < particles.size(); ++i) {
        Ref<GDParticle> gp = particles[i];
        if (!gp.is_valid()) continue;
        gp->build();
    }

    // Build and take the constraints
    std::vector<sim::Constraint*> sim_constraints;
    sim_constraints.reserve(constraints.size());
    for (int i = 0; i < constraints.size(); ++i) {
        Ref<GDConstraint> gc = constraints[i];
        if (!gc.is_valid()) continue;
        gc->build();
        sim_constraints.push_back(gc->take_sim_constraint());
    }
    
    // Take the particles
    for (int i = 0; i < particles.size(); ++i) {
        Ref<GDParticle> gp = particles[i];
        if (!gp.is_valid()) continue;
        sim_particles.push_back(gp->take_sim_particle());
    }

    soft_body = new sim::SoftBody(sim_particles, sim_constraints, friction, restitution);
}

void GDSoftBody::reset() {
    for (int i = 0; i < particles.size(); ++i) {
        Ref<GDParticle> gp = particles[i];
        if (!gp.is_valid()) continue;
        gp->reset();
    }
    for (int i = 0; i < constraints.size(); ++i) {
        Ref<GDConstraint> gc = constraints[i];
        if (!gc.is_valid()) continue;
        gc->reset();
    }
    if (soft_body) {
        delete soft_body;
        soft_body = nullptr;
    }
}
