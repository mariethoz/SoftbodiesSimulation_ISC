#include "2_GDSoftBody.h"

using namespace godot;

void GDSoftBody_2::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_unit","unit"), &GDSoftBody_2::set_unit);
    ClassDB::bind_method(D_METHOD("get_unit"), &GDSoftBody_2::get_unit);

    ClassDB::bind_method(D_METHOD("set_mass","mass"), &GDSoftBody_2::set_mass);
    ClassDB::bind_method(D_METHOD("get_mass"), &GDSoftBody_2::get_mass);
    ClassDB::bind_method(D_METHOD("set_radius","double"), &GDSoftBody_2::set_radius);
    ClassDB::bind_method(D_METHOD("get_radius"), &GDSoftBody_2::get_radius);
    
    ClassDB::bind_method(D_METHOD("set_friction","friction"), &GDSoftBody_2::set_friction);
    ClassDB::bind_method(D_METHOD("get_friction"), &GDSoftBody_2::get_friction);
    ClassDB::bind_method(D_METHOD("set_restitution","restitution"), &GDSoftBody_2::set_restitution);
    ClassDB::bind_method(D_METHOD("get_restitution"), &GDSoftBody_2::get_restitution);

    ClassDB::bind_method(D_METHOD("set_stiffness","stiffness"), &GDSoftBody_2::set_stiffness);
    ClassDB::bind_method(D_METHOD("get_stiffness"), &GDSoftBody_2::get_stiffness);
    ClassDB::bind_method(D_METHOD("set_damping","damping"), &GDSoftBody_2::set_damping);
    ClassDB::bind_method(D_METHOD("get_damping"), &GDSoftBody_2::get_damping);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "unit"), "set_unit", "get_unit");

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "friction"), "set_friction", "get_friction");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "restitution"), "set_restitution", "get_restitution");

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "stiffness"), "set_stiffness", "get_stiffness");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "damping"), "set_damping", "get_damping");

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "particule_mass"), "set_mass", "get_mass");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "particles_radius"), "set_radius", "get_radius");
}

GDSoftBody_2::~GDSoftBody_2() {
    if (soft_body) {
        delete soft_body;
        soft_body = nullptr;
    }
}

void GDSoftBody_2::build() {
    std::cout << "Build SoftBody; ";
    if (soft_body) return;
    if (backed_ploygon.is_empty())
        backed_ploygon = get_polygon();
    Vector2 global_position = get_global_position();

    std::vector<sim::Vector2> border;

    std::cout << backed_ploygon.size() << " Convert Godot to C++ ";
    for (int i = 0; i < backed_ploygon.size(); i++) {
        sim::Vector2 v1 = convert::from_godot(backed_ploygon[i] + global_position);
        sim::Vector2 v2 = convert::from_godot(backed_ploygon[(i+1)%backed_ploygon.size()] + global_position);
        int n = std::max(1, (int)std::floor(sim::dist(v1, v2) / double(unit))); // number of subdivisions
        std::vector<sim::Vector2> segment = sim::divideSegment(v1, v2, n);
        for (auto s: segment)
            border.push_back(s);
    }
    if (!get_uv().is_empty()) {
        PackedVector2Array b;
        PackedVector2Array u;
        b.resize(border.size());
        u.resize(border.size());
        for (int i = 0; i < border.size(); i++) {
            auto p = convert::to_godot(border[i]);
            b[i] = p - global_position;
            u[i] = p;
        }
        set_polygon(b);
        set_uv(u);
        backed_ploygon = get_polygon();
    }

    std::cout << " Create SoftBody ";
    if (border.size() >= 1) {
        std::cout << " Factory\n";
        soft_body = sim::SoftBody::createFromPolygon(border, unit, mass, particles_radius, stiffness, damping, friction, restitution);
    } else {
        std::cout << " nullptr\n";
        soft_body = nullptr;
    }
}

void GDSoftBody_2::update(std::vector<sim::Particle*> new_border) {
    PackedVector2Array b;
    Vector2 global_position = get_global_position();
    
    b.resize(new_border.size());
    for (int i = 0; i < new_border.size(); i++) {
        b[i] = convert::to_godot(new_border[i]->getPosition()) - global_position;
    }
    set_polygon(b);
}

void GDSoftBody_2::reset() {
    if (soft_body) {
        delete soft_body;
        soft_body = nullptr;
    }
}
