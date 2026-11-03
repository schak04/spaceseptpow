#pragma once

#include <raylib.h>

#include "constants.hpp"

#define MAX_REGULAR_ENEMIES 20
#define MAX_SPECIAL_ENEMIES 5

struct RegularEnemy {
    int HP = 100;
    Vector2 pos = {0.0f, 0.0f};
    Vector2 dimensions = {100, 100};
    Vector2 velocity = {0.0f, 0.0f};
    bool active = false;
};

struct SpecialEnemy {
    Texture2D texture;
    int HP = 200;
    Vector2 pos = {0.0f, 0.0f};
    Vector2 dimensions = {100, 100};
    Vector2 velocity = {0.0f, 0.0f};
    const float eneryDropChance = 0.05;
};

extern RegularEnemy regularEnemies[MAX_REGULAR_ENEMIES];
extern SpecialEnemy specialEnemies[MAX_SPECIAL_ENEMIES];
extern Texture2D regularEnemyTexture;

void spawnEnemies();
void updateEnemies();
void drawEnemies();
