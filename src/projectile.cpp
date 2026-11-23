#include "projectile.hpp"

#include "player.hpp"

Projectile projectiles[MAX_PROJECTILES];
BossProjectile bossProjectiles[MAX_BOSS_PROJECTILES];
Sound shootingSound;
Sound collisionSound;

void resetProjectiles() {
    for (Projectile& projectile : projectiles) projectile.active = false;
    resetBossProjectiles();
}

void resetBossProjectiles() {
    for (BossProjectile& projectile : bossProjectiles) projectile.active = false;
}

void spawnBossProjectile(Vector2 pos, Vector2 velocity) {
    for (BossProjectile& projectile : bossProjectiles) {
        if (!projectile.active) {
            projectile.pos = pos;
            projectile.velocity = velocity;
            projectile.active = true;
            PlaySound(shootingSound);
            break;
        }
    }
}

const float projectileSpeed = 2000.0f;
void moveProjectiles() {  // TODO: separate spawning and updating projectiles later
    const float dt = GetFrameTime();

    // find a free slot in the projectiles array to spawn a projectile
    // and skip firing if all slots are occupied
    if (player.active && IsKeyPressed(KEY_SPACE)) {
        for (int i = 0; i < MAX_PROJECTILES; i++) {
            if (!projectiles[i].active) {
                // projectile spawning
                projectiles[i].pos = {player.pos.x + (player.dimensions.x / 2.0f), player.pos.y};
                projectiles[i].velocity = {0.0f, -projectileSpeed};
                projectiles[i].active = true;
                PlaySound(shootingSound);
                break;
            }
        }
    }

    // move active projectiles and remove when out of screen
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!projectiles[i].active) continue;

        projectiles[i].pos.x += projectiles[i].velocity.x * dt;
        projectiles[i].pos.y += projectiles[i].velocity.y * dt;

        // remove projectile if its
        // bottom edge crosses the game's upper boundary
        // or top edge -> lower boundary
        // or right edge -> left boundary
        // or left edge -> right boundary
        if (projectiles[i].pos.y + projectiles[i].radius < 0.0f ||
            projectiles[i].pos.y - projectiles[i].radius > (float)SCREEN_HEIGHT ||
            projectiles[i].pos.x + projectiles[i].radius < 0.0f ||
            projectiles[i].pos.x - projectiles[i].radius > (float)SCREEN_WIDTH) {
            projectiles[i].active = false;
        }
    }

    for (BossProjectile& projectile : bossProjectiles) {
        if (!projectile.active) continue;

        projectile.pos.x += projectile.velocity.x * dt;
        projectile.pos.y += projectile.velocity.y * dt;
        if (projectile.pos.y - projectile.radius > (float)SCREEN_HEIGHT ||
            projectile.pos.y + projectile.radius < 0.0f ||
            projectile.pos.x - projectile.radius > (float)SCREEN_WIDTH ||
            projectile.pos.x + projectile.radius < 0.0f) {
            projectile.active = false;
        }
    }
}

void drawProjectiles() {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (projectiles[i].active) {
            DrawCircleV(projectiles[i].pos, projectiles[i].radius, GOLD);
        }
    }

    for (const BossProjectile& projectile : bossProjectiles) {
        if (projectile.active) DrawCircleV(projectile.pos, projectile.radius, RED);
    }
}
