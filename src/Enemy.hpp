#pragma once

#include "Vec2.hpp"

struct Enemy
{
    Vec2 position;
    float w = 40.0f;
    float h = 40.0f;
    float speed = 160.0f;
};
