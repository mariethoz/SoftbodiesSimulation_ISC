#pragma once
#include "Vector2.h"
#include <godot_cpp/classes/node2d.hpp>

namespace convert {
    inline godot::Vector2 to_godot(const sim::Vector2 &v) {
        return godot::Vector2(v.x, -v.y);
    }

    inline sim::Vector2 from_godot(const godot::Vector2 &v) {
        return sim::Vector2(v.x, -v.y);
    }
}
