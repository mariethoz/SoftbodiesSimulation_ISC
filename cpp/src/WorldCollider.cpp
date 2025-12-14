#include "WorldCollider.h"

#include "PlaneWorldCollider.h"
#include "CircleWorldCollider.h"

using namespace sim;

WorldCollider* WorldCollider::from_json(json data) {
    COLLIDER_TYPE ct = data["ColliderType"];
    Vector2 point = Vector2::from_json(data["point"]);
    double distance = data["distance"];
    switch (ct)
    {
    case COLLIDER_TYPE::OuterCircleColliderType:
        return new OuterCircleCollider(point, distance);
        break;
    case COLLIDER_TYPE::InnerCircleCollideTyper:
        return new InnerCircleCollider(point, distance);
        break;
    case COLLIDER_TYPE::PlaneColliderType:
    default:
        return new PlaneCollider(point, distance);
        break;
    }
};
