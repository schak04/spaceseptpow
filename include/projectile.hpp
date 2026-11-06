#pragma once

#include <raylib.h>

#include "constants.hpp"

#define MAX_PROJECTILES 100
#define PROJECTILE_IMPACT 50

struct Projectile {
    Vector2 pos = {0.0f, 0.0f};
    Vector2 velocity = {0.0f, 0.0f};
    float radius = 4.0f;
    bool active = false;
};

extern Projectile projectiles[MAX_PROJECTILES];

void moveProjectiles();
void drawProjectiles();
