#pragma once

#include <raylib.h>

#include "constants.hpp"

#define MAX_REGULAR_ENEMIES 40
#define MAX_SPECIAL_ENEMIES 20
#define REGULAR_ENEMY_HP 100
#define SPECIAL_ENEMY_HP 200
#define MAX_ENERGY_PICKUPS MAX_SPECIAL_ENEMIES

struct RegularEnemy {
    int HP = REGULAR_ENEMY_HP;
    Vector2 pos = {0.0f, 0.0f};
    Vector2 dimensions = {100, 100};
    Vector2 velocity = {0.0f, 0.0f};
    bool active = false;
};

struct SpecialEnemy {
    int HP = SPECIAL_ENEMY_HP;
    Vector2 pos = {0.0f, 0.0f};
    Vector2 dimensions = {100, 100};
    Vector2 velocity = {0.0f, 0.0f};
    const float eneryDropChance = 0.5;
    bool active = false;
    float randomZigZagMotionTimer = 0.0f;
    int randomness = 0;  // for random zig-zag motion
};

struct EnergyPickup {
    Vector2 pos = {0.0f, 0.0f};
    float radius = 10.0f;
    float timeRemaining = 5.0f;
    bool active = false;
};

extern RegularEnemy regularEnemies[MAX_REGULAR_ENEMIES];
extern SpecialEnemy specialEnemies[MAX_SPECIAL_ENEMIES];
extern EnergyPickup energyPickups[MAX_ENERGY_PICKUPS];
extern Texture2D regularEnemyTexture;
extern Texture2D specialEnemyTexture;

void spawnRegularEnemies();
void moveRegularEnemies();
void drawRegularEnemies();
void spawnSpecialEnemies();
void moveSpecialEnemies();
void drawSpecialEnemies();
void drawEnergyPickups();
void updateEnergyPickups();
void resetEnemies();
void startWave(int waveNumber);
bool isWaveComplete();
void registerRegularEnemyDefeat();
void registerSpecialEnemyDefeat();
void queueRegularEnemyReplacement();
void queueSpecialEnemyReplacement();
