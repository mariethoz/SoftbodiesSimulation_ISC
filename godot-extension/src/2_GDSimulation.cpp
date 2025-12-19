//GDSimulation.cpp
#include "2_GDSimulation.h"
#include "2_GDSoftBody.h"
#include "2_GDCollider.h"
#include "GDVector2.h"

#include <PlaneWorldCollider.h>
#include <CircleWorldCollider.h>

using namespace godot;

void GDSimulation_2::draw_simulation() {
    for (auto body : simulation.getBodies()) {
        bodies[body]->update(body->getBorder());
        if (_verbose){
            for (auto c: body->getConstraints()) {
                Vector2 p1 = convert::to_godot(c->getPart1());
                Vector2 p2 = convert::to_godot(c->getPart2());
                draw_line(p1,p2,Color(1,1,1), 1.0);
            }
            for (auto p : body->getParticles()) {
                Vector2 pos = convert::to_godot(p->getPosition()) ;
                float radius = p->getRadius();
                draw_circle(pos, radius, Color(1,0,0));
            }
            for (auto p : body->getBorder()) {
                Vector2 pos = convert::to_godot(p->getPosition()) ;
                float radius = p->getRadius();
                draw_circle(pos, radius, Color(0,1,0));
            }
        }
    }
}

/// @brief Binds the methods and properties of the GDSimulation_2 class to Godot.
void godot::GDSimulation_2::_bind_methods() {
    ClassDB::bind_method(D_METHOD("reset_simulation"), &GDSimulation_2::reset_simulation);

    ClassDB::bind_method(D_METHOD("set_gravity", "gravity"), &GDSimulation_2::set_gravity);
    ClassDB::bind_method(D_METHOD("get_gravity"), &GDSimulation_2::get_gravity);
    ClassDB::bind_method(D_METHOD("set_debug", "bool"), &GDSimulation_2::set_debug);
    ClassDB::bind_method(D_METHOD("get_debug"), &GDSimulation_2::get_debug);

    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "gravity"), "set_gravity", "get_gravity");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "draw_debug"), "set_debug", "get_debug");
}

void godot::GDSimulation_2::build() {
    simulation.clear();
    simulation.setGravity(convert::from_godot(gravity));

    bodies.clear();
    TypedArray<Node> childs = get_children();
    for (int i = 0; i < childs.size(); i++) {
        Node *child = Object::cast_to<Node>(childs[i]);
        if (!child) continue;

        GDSoftBody_2* sb = Object::cast_to<GDSoftBody_2>(child);
        if (sb){
            sb->reset();
            sb->build();
            if (sb->get_sim_softbody()){
                bodies.insert({sb->get_sim_softbody(), sb});
                simulation.addBody(sb->take_sim_softbody());
            }
        }
        GDCollider* wc = Object::cast_to<GDCollider>(child);
        if (wc) {
            wc->reset();
            wc->build();
            if (wc->get_sim_collider()){
                simulation.addCollider(wc->take_sim_collider());
                colliders.push_back(wc);
            }
        }
    }
}
