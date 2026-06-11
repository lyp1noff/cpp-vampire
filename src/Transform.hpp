#pragma once

#include "Vec2.hpp"

inline Vec2 worldToScreen(Vec2 pos, Vec2 cameraPosition) {
    return {pos.x - cameraPosition.x, pos.y - cameraPosition.y};
}
