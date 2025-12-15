#include "2_GDCollider.h"

using namespace godot;

void GDCollider::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_point", "point"), &GDCollider::set_point);
    ClassDB::bind_method(D_METHOD("get_point"), &GDCollider::get_point);
    ClassDB::bind_method(D_METHOD("set_distance", "distance"), &GDCollider::set_distance);
    ClassDB::bind_method(D_METHOD("get_distance"), &GDCollider::get_distance);
    ClassDB::bind_method(D_METHOD("set_collider", "collider"), &GDCollider::set_collider);
    ClassDB::bind_method(D_METHOD("get_collider"), &GDCollider::get_collider);

    ClassDB::bind_method(D_METHOD("set_friction", "friction"), &GDCollider::set_friction);
    ClassDB::bind_method(D_METHOD("get_friction"), &GDCollider::get_friction);
    ClassDB::bind_method(D_METHOD("set_restitution", "restitution"), &GDCollider::set_restitution);
    ClassDB::bind_method(D_METHOD("get_restitution"), &GDCollider::get_restitution);

    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "center/normal"), "set_point", "get_point");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "radius/distance"), "set_distance", "get_distance");
    ADD_PROPERTY(
        PropertyInfo(Variant::INT, "collider_type",
        PROPERTY_HINT_ENUM, "PLANE,INNERCIRCLE,OUTERCIRCLE"),
        "set_collider", "get_collider"
    );

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "friction"), "set_friction", "get_friction");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "restitution"), "set_restitution", "get_restitution");
}

GDCollider::~GDCollider() {
    if (world_collider) {
        delete world_collider;
        world_collider = nullptr;
    }
}

void GDCollider::build() {
    switch (collider_type)
    {
    case INNERCIRCLE:
        world_collider = new sim::InnerCircleCollider(convert::from_godot(point), distance, friction, restitution);
        break;
    case OUTERCIRCLE:
        world_collider = new sim::OuterCircleCollider(convert::from_godot(point), distance, friction, restitution);
        break;
    case PLANE:
    default:
        world_collider = new sim::PlaneCollider( convert::from_godot(point), distance, friction, restitution);
        break;
    }
}

void GDCollider::reset() {
    if (world_collider) {
        delete world_collider;
        world_collider = nullptr;
    }
}

void godot::GDCollider::_draw() {
    switch (collider_type)
    {
    case INNERCIRCLE:
        draw_circle(point, distance, Color(0,0,1), false);
        break;
    case OUTERCIRCLE:
        draw_circle(point, distance, Color(0,0,1), true);
        break;
    case PLANE:
    default:
        Vector2 n = point;
        Vector2 origin = Vector2(0, -distance);

        draw_line(origin - n.rotated(Math_PI/2) * 500,
                origin + n.rotated(Math_PI/2) * 500,
                Color(0,0,1), 2);
        break;
    }
}
