#pragma once

#include <raylib.h>

#include "constants.hpp"

#define MAX_REGULAR_ENEMIES 40
#define MAX_SPECIAL_ENEMIES 10

struct RegularEnemy {
    int HP = 100;
    Vector2 pos = {0.0f, 0.0f};
    Vector2 dimensions = {100, 100};
    Vector2 velocity = {0.0f, 0.0f};
    bool active = false;
};

struct SpecialEnemy {
    int HP = 200;
    Vector2 pos = {0.0f, 0.0f};
    Vector2 dimensions = {100, 100};
    Vector2 velocity = {0.0f, 0.0f};
    const float eneryDropChance = 0.05;
    bool active = false;
    float randomZigZagMotionTimer = 0.0f;
    int randomness = 0;  // for random zig-zag motion
};

extern RegularEnemy regularEnemies[MAX_REGULAR_ENEMIES];
extern SpecialEnemy specialEnemies[MAX_SPECIAL_ENEMIES];
extern Texture2D regularEnemyTexture;
extern Texture2D specialEnemyTexture;

void spawnRegularEnemies();
void moveRegularEnemies();
void drawRegularEnemies();
void spawnSpecialEnemies();
void moveSpecialEnemies();
void drawSpecialEnemies();
