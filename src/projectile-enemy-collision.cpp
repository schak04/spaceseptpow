#include "projectile-enemy-collision.hpp"

#include "enemy.hpp"
#include "projectile.hpp"

void detectProjectileEnemyCollision() {
    for (RegularEnemy& enemy : regularEnemies) {
        if (!enemy.active) continue;

        // NOTE:
        // The enemy texture is 500 x 500 but the actual visible enemy occupies:
        // X: 104 to 396 and Y: 101 to 400
        // The texture can be thought of as a square containing an enemy with transparent padding
        // around it so the texture's boundaries and the visible sprite's boundaries are different.
        // Also, it's drawn with a 180 degree rotation which effectively flips the texture
        // both horizontally and vertically.
        // So, right edge -> visible left edge and vice versa
        // bottom edge -> visible top edge and vice versa

        // NOTE: for future reference
        // The bounds were determined by inspecting the texture's alpha channel in Aseprite.
        // Steps:
        // - Find the first and last non-transparent pixels along each axis.
        // - Use the pixel coordinates shown by Aseprite's status bar while
        //   hovering over the canvas.

        const float visibleLeft = 104.0f;
        const float visibleRight = 396.0f;
        const float visibleTop = 101.0f;
        const float visibleBottom = 400.0f;

        // Rectangle expects top-left coordinates
        Rectangle enemyRec = {
            enemy.pos.x - enemy.dimensions.x / 2.0f +
                (regularEnemyTexture.width - visibleRight) / regularEnemyTexture.width *
                    enemy.dimensions.x,
            enemy.pos.y - enemy.dimensions.y / 2.0f +
                (regularEnemyTexture.height - visibleBottom) / regularEnemyTexture.height *
                    enemy.dimensions.y,
            (visibleRight - visibleLeft) / regularEnemyTexture.width * enemy.dimensions.x,
            (visibleBottom - visibleTop) / regularEnemyTexture.height * enemy.dimensions.y};

        for (Projectile& projectile : projectiles) {
            if (!projectile.active) continue;

            if (CheckCollisionPointRec(projectile.pos, enemyRec)) {
                projectile.active = false;
                enemy.HP -= 50;

                if (enemy.HP <= 0) {
                    enemy.active = false;
                }
            }
        }
    }
}
