#pragma once

#include "Rect.hpp"

struct Player {
    Rect rect{
        .x = 640.0f,
        .y = 360.0f,
        .w = 64.0f,
        .h = 64.0f
    };
    float speed = 300.0f;
};
