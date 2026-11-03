#pragma once

#include <raylib.h>

#include "constants.hpp"

struct Player {
    Texture2D texture;
    Vector2 pos = {(float)SCREEN_WIDTH * (1.0f / 2.0f), (float)SCREEN_HEIGHT * (94.0f / 100.0f)};
    Vector2 dimensions = {80, 80};
    Vector2 velocity = {0.0f, 0.0f};
    Vector2 acceleration = {0.0f, 0.0f};
};

extern Player player;

void movePlayer();
void drawPlayer();
