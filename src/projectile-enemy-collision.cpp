#include "projectile-enemy-collision.hpp"

#include "enemy.hpp"
#include "projectile.hpp"

// TODO: the enemy positioning still feels a little off, need to fix it later
void detectProjectileEnemyCollision() {
    for (RegularEnemy& enemy : regularEnemies) {
        if (!enemy.active) continue;

        // NOTE: Rectangle expects top-left coordinates
        Rectangle enemyRec = {enemy.pos.x - enemy.dimensions.x / 2.0f,
                              enemy.pos.y - enemy.dimensions.y / 2.0f, enemy.dimensions.x,
                              enemy.dimensions.y};

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
