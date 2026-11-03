#include "enemy.hpp"

RegularEnemy regularEnemies[MAX_REGULAR_ENEMIES];
SpecialEnemy specialEnemies[MAX_SPECIAL_ENEMIES];
Texture2D regularEnemyTexture;

float spawnTimer = 0.0f;  // to track time elapsed over frames
const float spawnInterval = 1.5f;
const float regularEnemySpeed = 100.0f;
void spawnEnemies() {
    spawnTimer += GetFrameTime();

    for (RegularEnemy& enemy : regularEnemies) {
        if ((spawnTimer >= spawnInterval) && !enemy.active) {
            spawnTimer = 0.0f;
            enemy.active = true;
            // NOTE:
            // GetRandomValue() uses a pseudo random number generator,
            // and produces a sequence of numbers that appears random but is deterministic.
            // Without seeding, it might give the same sequence every time the game is launched.
            // SetRandomSeed() changes the starting state of that generator.
            enemy.pos.x = (float)GetRandomValue(0, SCREEN_WIDTH - (int)enemy.dimensions.x);
            enemy.velocity = {0.0f, regularEnemySpeed};
            break;
        }
    }
}

void updateEnemies() {
    const float dt = GetFrameTime();

    for (RegularEnemy& enemy : regularEnemies) {
        if (!enemy.active) continue;

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
        }
    }
}

void drawEnemies() {
    for (const RegularEnemy& enemy : regularEnemies) {
        if (enemy.active) {
            // NOTE: source rect -> which portion of the texture to use
            // dest rect -> where & at what size to draw that portion on the screen

            // NOTE: Rectangle -> x, y, width, height
            Rectangle srcRect = {0.0f, 0.0f, (float)regularEnemyTexture.width,
                                 (float)regularEnemyTexture.height};
            Rectangle destRect = {enemy.pos.x, enemy.pos.y, enemy.dimensions.x, enemy.dimensions.y};
            Vector2 origin = {0.0f, 0.0f};
            float rotation = 180.0f;

            DrawTexturePro(regularEnemyTexture, srcRect, destRect, origin, rotation, WHITE);
        }
    }
}
