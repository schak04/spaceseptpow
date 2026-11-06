// TODO: prevent overlapping spawns
// smoothen random zig-zag motion of special enemies

#include "enemy.hpp"

RegularEnemy regularEnemies[MAX_REGULAR_ENEMIES];
SpecialEnemy specialEnemies[MAX_SPECIAL_ENEMIES];
Texture2D regularEnemyTexture;
Texture2D specialEnemyTexture;

float regularEnemySpawnTimer = 0.0f;
const float regularEnemySpawnInterval = 1.0f;
const float regularEnemySpeed = 100.0f;
float specialEnemySpawnTimer = 0.0f;
const float specialEnemySpawnInterval = 2.5f;
const float specialEnemySpeed = 120.0f;

void spawnRegularEnemies() {
    regularEnemySpawnTimer += GetFrameTime();

    if (regularEnemySpawnTimer < regularEnemySpawnInterval) {
        return;
    } else {
        regularEnemySpawnTimer = 0.0f;
    }

    // TraceLog(LOG_INFO, "Spawn attempt. Timer: %.2f", regularEnemySpawnTimer);
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
            // and same with HP

            // pos represents the center
            enemy.HP = REGULAR_ENEMY_HP;
            enemy.pos.x = (float)GetRandomValue((int)(enemy.dimensions.x / 2),
                                                SCREEN_WIDTH - (int)(enemy.dimensions.x / 2.0f));
            enemy.pos.y = -enemy.dimensions.y / 2.0f;

            enemy.velocity = {0.0f, regularEnemySpeed};
            break;
        }
    }
}

void spawnSpecialEnemies() {
    specialEnemySpawnTimer += GetFrameTime();

    if (specialEnemySpawnTimer < specialEnemySpawnInterval) {
        return;
    } else {
        specialEnemySpawnTimer = 0.0f;
    }

    for (SpecialEnemy& enemy : specialEnemies) {
        if (!enemy.active) {
            enemy.active = true;
            enemy.HP = SPECIAL_ENEMY_HP;
            enemy.pos.x = (float)GetRandomValue((int)(enemy.dimensions.x / 2),
                                                SCREEN_WIDTH - (int)(enemy.dimensions.x / 2.0f));
            enemy.pos.y = -enemy.dimensions.y / 2.0f;
            enemy.velocity = {0.0f, specialEnemySpeed};
            break;
        }
    }
}

void moveRegularEnemies() {
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

// to facilitate random zig-zag motion
const float specialEnemyRandomZigZagMotionInterval = 1.5f;

void moveSpecialEnemies() {
    const float dt = GetFrameTime();

    for (SpecialEnemy& enemy : specialEnemies) {
        enemy.randomZigZagMotionTimer += dt;
        if (!enemy.active) continue;

        // basic and abrupt random zig-zag motion for now
        // TODO: improve and smoothen this later
        if (enemy.randomZigZagMotionTimer < specialEnemyRandomZigZagMotionInterval) {
            enemy.randomness = 0;
        } else {
            enemy.randomness = GetRandomValue(-10000, 10000);
            enemy.randomZigZagMotionTimer = 0.0f;
        }
        enemy.pos.x += (enemy.velocity.x + enemy.randomness) * dt;
        enemy.pos.y += enemy.velocity.y * dt;

        if (enemy.pos.x + enemy.dimensions.x / 2.0f < 0.0f ||
            enemy.pos.x - enemy.dimensions.x / 2.0f > (float)SCREEN_WIDTH ||
            enemy.pos.y + enemy.dimensions.y / 2.0f < 0.0f ||
            enemy.pos.y - enemy.dimensions.y / 2.0f > (float)SCREEN_HEIGHT) {
            enemy.active = false;
        }
    }
}

void drawRegularEnemies() {
    for (const RegularEnemy& enemy : regularEnemies) {
        if (enemy.active) {
            // NOTE: source rect -> which portion of the texture to use
            // dest rect -> where & at what size to draw that portion on the screen

            // NOTE: Rectangle -> x, y, width, height
            Rectangle srcRect = {0.0f, 0.0f, (float)regularEnemyTexture.width,
                                 (float)regularEnemyTexture.height};
            Rectangle destRect = {enemy.pos.x, enemy.pos.y, enemy.dimensions.x, enemy.dimensions.y};
            Vector2 origin = {enemy.dimensions.x / 2.0f, enemy.dimensions.y / 2.0f};
            float rotation = 180.0f;

            DrawTexturePro(regularEnemyTexture, srcRect, destRect, origin, rotation, WHITE);
        }
    }
}

void drawSpecialEnemies() {
    for (const SpecialEnemy& enemy : specialEnemies) {
        if (enemy.active) {
            Rectangle srcRect = {0.0f, 0.0f, (float)specialEnemyTexture.width,
                                 (float)specialEnemyTexture.height};
            Rectangle destRect = {enemy.pos.x, enemy.pos.y, enemy.dimensions.x, enemy.dimensions.y};
            Vector2 origin = {enemy.dimensions.x / 2.0f, enemy.dimensions.y / 2.0f};
            float rotation = 180.0f;

            DrawTexturePro(specialEnemyTexture, srcRect, destRect, origin, rotation, WHITE);
        }
    }
}
