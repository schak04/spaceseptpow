#include "projectile.hpp"

#include "player.hpp"

Projectile projectiles[MAX_PROJECTILES];

const float projectileSpeed = 2000.0f;
void moveProjectiles() {  // TODO: separate spawning and updating projectiles later
    const float dt = GetFrameTime();

    // find a free slot in the projectiles array to spawn a projectile
    // and skip firing if all slots are occupied
    if (IsKeyPressed(KEY_SPACE)) {
        for (int i = 0; i < MAX_PROJECTILES; i++) {
            if (!projectiles[i].active) {
                // projectile spawning
                projectiles[i].pos = {player.pos.x + (player.dimensions.x / 2.0f), player.pos.y};
                projectiles[i].velocity = {0.0f, -projectileSpeed};
                projectiles[i].active = true;
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
}

void drawProjectiles() {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (projectiles[i].active) {
            DrawCircleV(projectiles[i].pos, projectiles[i].radius, GOLD);
        }
    }
}
