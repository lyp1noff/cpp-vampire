#pragma once

#include "Vec2.hpp"

struct Rect
{
    float x = 0.0f;
    float y = 0.0f;
    float w = 0.0f;
    float h = 0.0f;

    Vec2 Position() const
    {
        return Vec2{x, y};
    }

    Vec2 Center() const
    {
        return Vec2{x + w * 0.5f, y + h * 0.5f};
    }
};

inline bool checkCollision(Rect a, Rect b)
{
    return a.x < b.x + b.w &&
           a.x + a.w > b.x &&
           a.y < b.y + b.h &&
           a.y + a.h > b.y;
};