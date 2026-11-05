#include <raylib.h>

#include <ctime>

#include "constants.hpp"
#include "enemy.hpp"
#include "player.hpp"
#include "projectile.hpp"

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_NAME);
    SetTargetFPS(60);

    // change the starting state of the random number generator used by GetRandomValue()
    // so it's truly random
    SetRandomSeed((unsigned int)time(nullptr));

    // TODO: add error handling later sometime
    player.texture = LoadTexture("assets/player/player-spaceship.png");

    regularEnemyTexture = LoadTexture("assets/enemies/regular/regular-enemy-spaceship.png");
    specialEnemyTexture = LoadTexture("assets/enemies/special/special-enemy-spaceship.png");

    while (!WindowShouldClose()) {
        movePlayer();
        spawnRegularEnemies();
        spawnSpecialEnemies();
        moveRegularEnemies();
        moveSpecialEnemies();
        moveProjectiles();

        BeginDrawing();

        ClearBackground(BLACK);

        drawProjectiles();
        drawPlayer();
        drawRegularEnemies();
        drawSpecialEnemies();

        EndDrawing();
    }

    UnloadTexture(player.texture);
    UnloadTexture(regularEnemyTexture);
    UnloadTexture(specialEnemyTexture);
    CloseWindow();

    return 0;
}
