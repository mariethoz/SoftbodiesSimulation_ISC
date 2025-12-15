#include "GDParticle.h"

using namespace godot;

void GDParticle::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_position","position"), &GDParticle::set_position);
    ClassDB::bind_method(D_METHOD("get_position"), &GDParticle::get_position);

    ClassDB::bind_method(D_METHOD("set_mass","mass"), &GDParticle::set_mass);
    ClassDB::bind_method(D_METHOD("get_mass"), &GDParticle::get_mass);
    
    ClassDB::bind_method(D_METHOD("set_radius","radius"), &GDParticle::set_radius);
    ClassDB::bind_method(D_METHOD("get_radius"), &GDParticle::get_radius);

    ClassDB::bind_method(D_METHOD("set_pinned","pinned"), &GDParticle::set_pinned);
    ClassDB::bind_method(D_METHOD("is_pinned"), &GDParticle::is_pinned);

    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "position"), "set_position", "get_position");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "mass"), "set_mass", "get_mass");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "radius"), "set_radius", "get_radius");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "pinned"), "set_pinned", "is_pinned");
}

GDParticle::~GDParticle() {
    if (particle) {
        delete particle;
        particle = nullptr;
    }
}

void GDParticle::build() {
    if (particle) return; // already built
    sim::Vector2 pos = convert::from_godot(position_gd);
    particle = new sim::Particle(pos, mass, radius, pinned);
}

void godot::GDParticle::reset() {
    if (particle) {
        delete particle;
        particle = nullptr;
    }
}
