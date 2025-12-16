//GDVector2.h
#pragma once
#include "Vector2.h"
#include <godot_cpp/classes/node2d.hpp>

namespace convert {
    /**
     * @brief Convert between sim::Vector2 and godot::Vector2
     * 
     * The conversion accounts for the difference in coordinate systems:
     * Godot uses a Y-down system, while the simulation uses a Y-up system.
     */
    inline godot::Vector2 to_godot(const sim::Vector2 &v) {
        return godot::Vector2(v.x, -v.y);
    }

    /**
     * @brief Convert between godot::Vector2 and sim::Vector2
     * 
     * The conversion accounts for the difference in coordinate systems:
     * Godot uses a Y-down system, while the simulation uses a Y-up system.
     */
    inline sim::Vector2 from_godot(const godot::Vector2 &v) {
        return sim::Vector2(v.x, -v.y);
    }
}
