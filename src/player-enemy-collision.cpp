#include "player-enemy-collision.hpp"

#include "enemy.hpp"
#include "player.hpp"

void detectPlayerEnemyCollision() {
    if (player.HP <= 0) return;

    // NOTE:
    // The player texture is 500 x 500 but the actual visible player occupies:
    // X: 51 to 449 and Y: 114 to 386
    // The texture can be thought of as a square containing the player with transparent padding
    // around it so the texture's boundaries and the visible sprite's boundaries are different.

    // NOTE: for future reference
    // The bounds were determined by inspecting the texture's alpha channel in Aseprite.
    // Steps:
    // - Find the first and last non-transparent pixels along each axis.
    // - Use the pixel coordinates shown by Aseprite's status bar while
    //   hovering over the canvas.

    const float playerVisibleLeft = 51.0f;
    const float playerVisibleRight = 449.0f;
    const float playerVisibleTop = 114.0f;
    const float playerVisibleBottom = 386.0f;

    Rectangle playerRec = {
        player.pos.x + playerVisibleLeft / player.texture.width * player.dimensions.x,
        player.pos.y + playerVisibleTop / player.texture.height * player.dimensions.y,
        (playerVisibleRight - playerVisibleLeft) / player.texture.width * player.dimensions.x,
        (playerVisibleBottom - playerVisibleTop) / player.texture.height * player.dimensions.y};

    for (RegularEnemy& enemy : regularEnemies) {
        if (!enemy.active) continue;

        // the enemy texture is 500 x 500 but the actual visible enemy occupies:
        // X: 104 to 396 and Y: 101 to 400

        const float enemyVisibleLeft = 104.0f;
        const float enemyVisibleRight = 396.0f;
        const float enemyVisibleTop = 101.0f;
        const float enemyVisibleBottom = 400.0f;

        Rectangle enemyRec = {
            enemy.pos.x - enemy.dimensions.x / 2.0f +
                (regularEnemyTexture.width - enemyVisibleRight) / regularEnemyTexture.width *
                    enemy.dimensions.x,
            enemy.pos.y - enemy.dimensions.y / 2.0f +
                (regularEnemyTexture.height - enemyVisibleBottom) / regularEnemyTexture.height *
                    enemy.dimensions.y,
            (enemyVisibleRight - enemyVisibleLeft) / regularEnemyTexture.width * enemy.dimensions.x,
            (enemyVisibleBottom - enemyVisibleTop) / regularEnemyTexture.height *
                enemy.dimensions.y};

        if (CheckCollisionRecs(playerRec, enemyRec)) {
            player.HP -= ENEMY_IMPACT;
            enemy.active = false;
            queueRegularEnemyReplacement();

            if (player.HP <= 0) {
                player.active = false;
                return;
            }
        }
    }

    for (SpecialEnemy& enemy : specialEnemies) {
        if (!enemy.active) continue;

        // the special enemy texture is 500 x 500 but the actual visible enemy occupies
        // X: 71 to 429 and Y: 102 to 398

        const float enemyVisibleLeft = 71.0f;
        const float enemyVisibleRight = 429.0f;
        const float enemyVisibleTop = 102.0f;
        const float enemyVisibleBottom = 398.0f;

        Rectangle enemyRec = {
            enemy.pos.x - enemy.dimensions.x / 2.0f +
                (specialEnemyTexture.width - enemyVisibleRight) / specialEnemyTexture.width *
                    enemy.dimensions.x,
            enemy.pos.y - enemy.dimensions.y / 2.0f +
                (specialEnemyTexture.height - enemyVisibleBottom) / specialEnemyTexture.height *
                    enemy.dimensions.y,
            (enemyVisibleRight - enemyVisibleLeft) / specialEnemyTexture.width * enemy.dimensions.x,
            (enemyVisibleBottom - enemyVisibleTop) / specialEnemyTexture.height *
                enemy.dimensions.y};

        if (CheckCollisionRecs(playerRec, enemyRec)) {
            player.HP -= ENEMY_IMPACT;
            enemy.active = false;
            queueSpecialEnemyReplacement();

            if (player.HP <= 0) {
                player.active = false;
                return;
            }
        }
    }

    // pickup collection
    for (EnergyPickup& pickup : energyPickups) {
        if (!pickup.active) continue;

        if (CheckCollisionCircleRec(pickup.pos, pickup.radius, playerRec)) {
            player.HP += PICKUP_AMOUNT;
            if (player.HP > PLAYER_HP) player.HP = PLAYER_HP;
            pickup.active = false;
        }
    }
}
