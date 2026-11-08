#pragma once

#include <raylib.h>

#include "constants.hpp"

#define PLAYER_HP 500
#define ENEMY_IMPACT 50  // on collision

struct Player {
    int HP = PLAYER_HP;
    Texture2D texture;
    Vector2 pos = {(float)SCREEN_WIDTH * (1.0f / 2.0f), (float)SCREEN_HEIGHT * (94.0f / 100.0f)};
    Vector2 dimensions = {80, 80};
    Vector2 velocity = {0.0f, 0.0f};
    Vector2 acceleration = {0.0f, 0.0f};
    bool active = true;
};

extern Player player;

void movePlayer();
void drawPlayer();
