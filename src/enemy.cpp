#include "enemy.hpp"

RegularEnemy regularEnemies[MAX_REGULAR_ENEMIES];
SpecialEnemy specialEnemies[MAX_SPECIAL_ENEMIES];
Texture2D regularEnemyTexture;

float spawnTimer = 0.0f;  // to track time elapsed over frames
const float spawnInterval = 1.5f;
const float regularEnemySpeed = 100.0f;

void spawnEnemies() {
    spawnTimer += GetFrameTime();

    if (spawnTimer < spawnInterval) {
        return;
    } else {
        spawnTimer = 0.0f;
    }

    // TraceLog(LOG_INFO, "Spawn attempt. Timer: %.2f", spawnTimer);
    for (RegularEnemy& enemy : regularEnemies) {
        if (!enemy.active) {
            enemy.active = true;
            // TraceLog(LOG_INFO, "Enemy spawned!");
            // NOTE:
            // GetRandomValue() uses a pseudo random number generator,
            // and produces a sequence of numbers that appears random but is deterministic.
            // Without seeding, it might give the same sequence every time the game is launched.
            // SetRandomSeed() changes the starting state of that generator.

            // NOTE:
            // reusing an inactive enemy slot does NOT reset its previous state
            // its position, velocity, HP, etc. retain their old values
            // that is why it is important to explicitly initialise (reinitialise)
            // the necessary fields whenever spawning an enemy

            // lack of pos.y reinit caused non-continuous spawning
            // because when the enemy using this slot goes out of the screen
            // and that slot becomes inactive
            // its pos.y still remains the same
            // so it's important to reinit it

            // pos represents the center
            enemy.pos.x = (float)GetRandomValue((int)(enemy.dimensions.x / 2),
                                                SCREEN_WIDTH - (int)(enemy.dimensions.x / 2.0f));
            enemy.pos.y = -enemy.dimensions.y / 2.0f;

            enemy.velocity = {0.0f, regularEnemySpeed};
            break;
        }
    }
}

void updateEnemies() {
    // int activeCount = 0;
    const float dt = GetFrameTime();

    for (RegularEnemy& enemy : regularEnemies) {
        if (!enemy.active) continue;

        // activeCount++;

        enemy.pos.x += enemy.velocity.x * dt;
        enemy.pos.y += enemy.velocity.y * dt;

        // remove enemy when it's outta the screen
        // conditions:
        // if the right edge crosses the left boundary of the screen
        // or left edge crosses right boundary
        // or bottom edge crosses upper boundary
        // or top edge crosses lower boundary
        if (enemy.pos.x + enemy.dimensions.x / 2.0f < 0.0f ||
            enemy.pos.x - enemy.dimensions.x / 2.0f > (float)SCREEN_WIDTH ||
            enemy.pos.y + enemy.dimensions.y / 2.0f < 0.0f ||
            enemy.pos.y - enemy.dimensions.y / 2.0f > (float)SCREEN_HEIGHT) {
            enemy.active = false;
            // activeCount--;
        }
    }

    // TraceLog(LOG_INFO, "Active enemies: %d", activeCount);
}

void drawEnemies() {
    for (const RegularEnemy& enemy : regularEnemies) {
        if (enemy.active) {
            // NOTE: source rect -> which portion of the texture to use
            // dest rect -> where & at what size to draw that portion on the screen

            // NOTE: Rectangle -> x, y, width, height
            Rectangle srcRect = {0.0f, 0.0f, (float)regularEnemyTexture.width,
                                 (float)regularEnemyTexture.height};
            Rectangle destRect = {enemy.pos.x - enemy.dimensions.x / 2.0f,
                                  enemy.pos.y - enemy.dimensions.y / 2.0f, enemy.dimensions.x,
                                  enemy.dimensions.y};
            Vector2 origin = {enemy.dimensions.x / 2.0f, enemy.dimensions.y / 2.0f};
            float rotation = 180.0f;

            DrawTexturePro(regularEnemyTexture, srcRect, destRect, origin, rotation, WHITE);
        }
    }
}
