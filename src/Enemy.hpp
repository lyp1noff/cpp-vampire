#pragma once

#include "Rect.hpp"

struct Enemy {
    Rect rect{
        .w = 40.0f,
        .h = 40.0f
    };
    float speed = 100.0f;
};
