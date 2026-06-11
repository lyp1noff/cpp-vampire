#pragma once

#include "Rect.hpp"

struct Player {
    Rect rect{
        .x = 640.0f,
        .y = 360.0f,
        .w = 40.0f,
        .h = 40.0f
    };
    float speed = 300.0f;
};
