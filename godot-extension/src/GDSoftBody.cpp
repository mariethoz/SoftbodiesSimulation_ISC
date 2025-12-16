#include "GDSoftBody.h"
#include "GDVector2.h"
#include <vector>
#include <cmath>
using namespace godot;

void GDSoftBody::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_friction","friction"), &GDSoftBody::set_friction);
    ClassDB::bind_method(D_METHOD("get_friction"), &GDSoftBody::get_friction);

    ClassDB::bind_method(D_METHOD("set_restitution","restitution"), &GDSoftBody::set_restitution);
    ClassDB::bind_method(D_METHOD("get_restitution"), &GDSoftBody::get_restitution);

    ClassDB::bind_method(D_METHOD("set_Stiffness","stiffness"), &GDSoftBody::set_Stiffness);
    ClassDB::bind_method(D_METHOD("get_stiffness"), &GDSoftBody::get_stiffness);

    ClassDB::bind_method(D_METHOD("set_damping","damping"), &GDSoftBody::set_damping);
    ClassDB::bind_method(D_METHOD("get_damping"), &GDSoftBody::get_damping);

    ClassDB::bind_method(D_METHOD("build"), &GDSoftBody::build);

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
        gc->set_stiffness(stiffness);
        gc->set_damping(damping);
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

void GDSoftBodyCustom::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_particles","particles"), &GDSoftBodyCustom::set_particles);
    ClassDB::bind_method(D_METHOD("get_particles"), &GDSoftBodyCustom::get_particles);

    ClassDB::bind_method(D_METHOD("set_constraints","constraints"), &GDSoftBodyCustom::set_constraints);
    ClassDB::bind_method(D_METHOD("get_constraints"), &GDSoftBodyCustom::get_constraints);

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
}

void GDSoftBodyTriange::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_corner_1","Vector2"), &GDSoftBodyTriange::set_corner_1);
    ClassDB::bind_method(D_METHOD("get_corner_1"), &GDSoftBodyTriange::get_corner_1);
    ClassDB::bind_method(D_METHOD("set_corner_2","Vector2"), &GDSoftBodyTriange::set_corner_2);
    ClassDB::bind_method(D_METHOD("get_corner_2"), &GDSoftBodyTriange::get_corner_2);
    ClassDB::bind_method(D_METHOD("set_corner_3","Vector2"), &GDSoftBodyTriange::set_corner_3);
    ClassDB::bind_method(D_METHOD("get_corner_3"), &GDSoftBodyTriange::get_corner_3);

    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "corner_1"), "set_corner_1", "get_corner_1");
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "corner_2"), "set_corner_2", "get_corner_2");
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "corner_3"), "set_corner_3", "get_corner_3");
}

void GDSoftBodyTriange::build() {
    // 1) Create particles
    particles.clear();
    constraints.clear();

    Ref<GDParticle> p1 = memnew(GDParticle);
    Ref<GDParticle> p2 = memnew(GDParticle);
    Ref<GDParticle> p3 = memnew(GDParticle);

    p1->set_position(corner_1);
    p2->set_position(corner_2);
    p3->set_position(corner_3);

    particles.push_back(p1);
    particles.push_back(p2);
    particles.push_back(p3);

    // 2) Create constraints
    auto make_constraint = [&](Ref<GDParticle> a, Ref<GDParticle> b) {
        Ref<GDConstraint> c = memnew(GDConstraint);
        c->set_part1(a);
        c->set_part2(b);
        constraints.push_back(c);
    };

    make_constraint(p1, p2);
    make_constraint(p2, p3);
    make_constraint(p3, p1);

    // 3) Run normal soft-body build
    GDSoftBody::build();
}

void GDSoftBodySquare::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_corner_1","Vector2"), &GDSoftBodySquare::set_corner_1);
    ClassDB::bind_method(D_METHOD("get_corner_1"), &GDSoftBodySquare::get_corner_1);
    ClassDB::bind_method(D_METHOD("set_corner_2","Vector2"), &GDSoftBodySquare::set_corner_2);
    ClassDB::bind_method(D_METHOD("get_corner_2"), &GDSoftBodySquare::get_corner_2);
    ClassDB::bind_method(D_METHOD("set_corner_3","Vector2"), &GDSoftBodySquare::set_corner_3);
    ClassDB::bind_method(D_METHOD("get_corner_3"), &GDSoftBodySquare::get_corner_3);
    ClassDB::bind_method(D_METHOD("set_corner_4","Vector2"), &GDSoftBodySquare::set_corner_4);
    ClassDB::bind_method(D_METHOD("get_corner_4"), &GDSoftBodySquare::get_corner_4);

    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "corner_1"), "set_corner_1", "get_corner_1");
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "corner_2"), "set_corner_2", "get_corner_2");
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "corner_3"), "set_corner_3", "get_corner_3");
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "corner_4"), "set_corner_4", "get_corner_4");
}

void GDSoftBodySquare::build() {
    // 1) Create particles
    particles.clear();
    constraints.clear();

    Ref<GDParticle> p1 = memnew(GDParticle);
    Ref<GDParticle> p2 = memnew(GDParticle);
    Ref<GDParticle> p3 = memnew(GDParticle);
    Ref<GDParticle> p4 = memnew(GDParticle);

    p1->set_position(corner_1);
    p2->set_position(corner_2);
    p3->set_position(corner_3);
    p3->set_position(corner_4);

    particles.push_back(p1);
    particles.push_back(p2);
    particles.push_back(p3);
    particles.push_back(p4);

    // 2) Create constraints
    auto make_constraint = [&](Ref<GDParticle> a, Ref<GDParticle> b) {
        Ref<GDConstraint> c = memnew(GDConstraint);
        c->set_part1(a);
        c->set_part2(b);
        constraints.push_back(c);
    };

    // Edges
    make_constraint(p1, p2);
    make_constraint(p2, p3);
    make_constraint(p3, p4);
    make_constraint(p4, p1);
    // Diagonals
    make_constraint(p1, p3);
    make_constraint(p2, p4);

    // 3) Run normal soft-body build
    GDSoftBody::build();
}

void GDSoftBodyPolygone::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_border", "border"), &GDSoftBodyPolygone::set_border);
    ClassDB::bind_method(D_METHOD("get_border"), &GDSoftBodyPolygone::get_border);
    ClassDB::bind_method(D_METHOD("set_unit","double"), &GDSoftBodyPolygone::set_unit);
    ClassDB::bind_method(D_METHOD("get_unit"), &GDSoftBodyPolygone::get_unit);
    ClassDB::bind_method(D_METHOD("set_radius","double"), &GDSoftBodyPolygone::set_radius);
    ClassDB::bind_method(D_METHOD("get_radius"), &GDSoftBodyPolygone::get_radius);
    ClassDB::bind_method(D_METHOD("set_mass","double"), &GDSoftBodyPolygone::set_mass);
    ClassDB::bind_method(D_METHOD("get_mass"), &GDSoftBodyPolygone::get_mass);
    ClassDB::bind_method(D_METHOD("set_stiffness","double"), &GDSoftBodyPolygone::set_stiffness);
    ClassDB::bind_method(D_METHOD("get_stiffness"), &GDSoftBodyPolygone::get_stiffness);
    ClassDB::bind_method(D_METHOD("set_damping","double"), &GDSoftBodyPolygone::set_damping);
    ClassDB::bind_method(D_METHOD("get_damping"), &GDSoftBodyPolygone::get_damping);

    ADD_PROPERTY(
        PropertyInfo(Variant::PACKED_VECTOR2_ARRAY, "border",
            PropertyHint::PROPERTY_HINT_ARRAY_TYPE, "godot::Vector2"),
        "set_border", "get_border"
    );
    ADD_PROPERTY(PropertyInfo(Variant::INT, "unit"), "set_unit", "get_unit");    
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "particles_radius"), "set_radius", "get_radius");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "mass"), "set_mass", "get_mass");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "stiffness"), "set_stiffness", "get_stiffness");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "damping"), "set_damping", "get_damping");
}

void GDSoftBodyPolygone::build() {
    std::vector<sim::Vector2> polygone;

    for (int i = 0; i < border.size(); i++) {
        Vector2 v = border[i];
        polygone.push_back(convert::from_godot(v));
    }

    if (polygone.size() >= 1)
        soft_body = sim::SoftBody::createFromPolygon(polygone, unit);
    else
        soft_body = nullptr;
}