#pragma once

inline void resolveSeparation(Rect &A, Rect &B, float aCorrection = 0.5f, float bCorrection = 0.5f) {
    Vec2 centerA = A.Center();
    Vec2 centerB = B.Center();
    Vec2 delta = {centerB.x - centerA.x, centerB.y - centerA.y};

    float radiusA = A.w * 0.4f;
    float radiusB = B.w * 0.4f;
    float minDistance = radiusA + radiusB;

    float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);

    if (distance >= minDistance)
        return;

    if (distance < 0.001f)
        return;

    float overlap = minDistance - distance;
    Vec2 normal = {delta.x / distance, delta.y / distance};

    float aPush = overlap * aCorrection;
    float bPush = overlap * bCorrection;
    A.x -= normal.x * aPush;
    A.y -= normal.y * aPush;
    B.x += normal.x * bPush;
    B.y += normal.y * bPush;
}
