#pragma once

#include <raylib.h>

#include "constants.hpp"

#define MAX_PROJECTILES 100
#define PROJECTILE_IMPACT 50
#define REGULAR_ENEMY_DEFEAT_POINTS 5
#define SPECIAL_ENEMY_DEFEAT_POINTS 10
#define BOSS_ENEMY_DEFEAT_POINTS 100
#define MAX_BOSS_PROJECTILES 20

struct Projectile {
    Vector2 pos = {0.0f, 0.0f};
    Vector2 velocity = {0.0f, 0.0f};
    float radius = 4.0f;
    bool active = false;
};

struct BossProjectile {
    Vector2 pos = {0.0f, 0.0f};
    Vector2 velocity = {0.0f, 0.0f};
    float radius = 8.0f;
    bool active = false;
};

extern Projectile projectiles[MAX_PROJECTILES];
extern BossProjectile bossProjectiles[MAX_BOSS_PROJECTILES];
extern Sound shootingSound;
extern Sound collisionSound;

void moveProjectiles();
void drawProjectiles();
void resetProjectiles();
void spawnBossProjectile(Vector2 pos, Vector2 velocity);
void resetBossProjectiles();
