#include "SoftBody.h"
#include <unordered_map>
#include <array>

using namespace sim;

SoftBody::SoftBody(
        std::vector<Particle *> particles,
        std::vector<Constraint *> constraints,
        double friction, double restitution
    )
    : border({}), particles(particles), constraints(constraints),
      friction(friction), restitution(restitution), mesh_unit(-1) {}

SoftBody::SoftBody(
        std::vector<Particle *> border,
        std::vector<Particle *> particles,
        std::vector<Constraint *> constraints,
        double friction, double restitution, int unit
    )
    : border(border), particles(particles), constraints(constraints),
      friction(friction), restitution(restitution), mesh_unit(unit) {}

SoftBody::~SoftBody() {
    std::cout << "SoftBody destroyed\n";
};

void SoftBody::applyForce(const Vector2 &f) {
    for (auto& p : particles) {
        p->applyForce(f);
    }
}

void SoftBody::solveConstraint() {
    for (auto& c : constraints) {
        c->applyConstraint();
    }
}


void SoftBody::update(double dt) {
    for (auto& p : particles) {
        p->update(dt);
    }
}

json sim::SoftBody::as_json() {
    json data;
    if (mesh_unit < 0) return data;
    data["firction"] = friction;
    data["restitution"] = restitution;
    data["mesh_unit"] = mesh_unit;
    data["radius"]  = border[0]->getRadius();
    data["mass"]  = border[0]->getMass();
    data["pinned"]  = border[0]->isPinned();
    data["stiffness"]  = constraints[0]->getStiffness();
    data["damping"]  = constraints[0]->getDamping();
    for (auto b: border){
        data["border"].push_back(b->as_json());
    }
    return data;
}

SoftBody *sim::SoftBody::from_json(json data) {
    std::vector<Vector2> border;
    for (auto b: data["border"])
        border.push_back(Particle::from_json(b));
    int unit = data["mesh_unit"];
    double mass =  data["mass"];
    double radius = data["radius"];
    bool is_pinned = data["pinned"];
    double stiffness = data["stiffness"];
    double damping = data["damping"];
    double friction = data["friction"];
    double restitution = data["restitution"];
    return createFromPolygon(border, unit,
        mass, radius,
        stiffness, damping,
        friction, restitution, is_pinned
    );
}
