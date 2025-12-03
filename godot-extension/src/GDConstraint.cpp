#include "GDConstraint.h"

using namespace godot;

void GDConstraint::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_part1","part1"), &GDConstraint::set_part1);
    ClassDB::bind_method(D_METHOD("get_part1"), &GDConstraint::get_part1);

    ClassDB::bind_method(D_METHOD("set_part2","part2"), &GDConstraint::set_part2);
    ClassDB::bind_method(D_METHOD("get_part2"), &GDConstraint::get_part2);

    ClassDB::bind_method(D_METHOD("set_stiffness","stiffness"), &GDConstraint::set_stiffness);
    ClassDB::bind_method(D_METHOD("get_stiffness"), &GDConstraint::get_stiffness);

    ClassDB::bind_method(D_METHOD("set_damping","damping"), &GDConstraint::set_damping);
    ClassDB::bind_method(D_METHOD("get_damping"), &GDConstraint::get_damping);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "part1", PROPERTY_HINT_RESOURCE_TYPE, "GDParticle"), "set_part1", "get_part1");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "part2", PROPERTY_HINT_RESOURCE_TYPE, "GDParticle"), "set_part2", "get_part2");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "stiffness"), "set_stiffness", "get_stiffness");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "damping"), "set_damping", "get_damping");
}

GDConstraint::~GDConstraint() {
    if (constraint) {
        delete constraint;
        constraint = nullptr;
    }
}

void GDConstraint::build() {
    if (constraint) return; // already built

    if (!part1.is_valid() || !part2.is_valid()) {
        ERR_PRINT("GDConstraint::build() missing part1 or part2");
        return;
    }

    // Ensure particles are built
    part1->build();
    part2->build();

    constraint = new sim::Constraint(part1->get_sim_particle(), part2->get_sim_particle(), stiffness, damping);
}

void godot::GDConstraint::reset() {
    if (constraint) {
        delete constraint;
        constraint = nullptr;
    }
}
