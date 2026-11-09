#include <raylib.h>

#include <cmath>
#include <ctime>

#include "constants.hpp"
#include "enemy.hpp"
#include "player-enemy-collision.hpp"
#include "player.hpp"
#include "projectile-enemy-collision.hpp"
#include "projectile.hpp"

enum class GameState { Start, Playing, GameOver };

void drawCenteredText(const char* text, int y, int fontSize, Color color) {
    DrawText(text, (SCREEN_WIDTH - MeasureText(text, fontSize)) / 2, y, fontSize, color);
}

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

    GameState gameState = GameState::Start;
    int currentWave = 0;
    bool waveIntermission = false;
    float waveIntermissionTimer = 0.0f;

    while (!WindowShouldClose()) {
        if (gameState == GameState::Start && IsKeyPressed(KEY_ENTER)) {
            gameState = GameState::Playing;
            currentWave = 1;
            startWave();
        } else if (gameState == GameState::GameOver && IsKeyPressed(KEY_ENTER)) {
            Texture2D playerTexture = player.texture;
            player = Player{};
            player.texture = playerTexture;
            resetEnemies();
            resetProjectiles();
            currentWave = 1;
            waveIntermission = false;
            startWave();
            gameState = GameState::Playing;
        }

        if (gameState == GameState::Playing) {
            if (waveIntermission) {
                // skipping gameplay updates to keep the cleared arena still during the warning
                waveIntermissionTimer -= GetFrameTime();
                if (waveIntermissionTimer <= 0.0f) {
                    waveIntermission = false;
                    currentWave++;
                    startWave();
                }
            } else {
                movePlayer();
                spawnRegularEnemies();
                spawnSpecialEnemies();
                moveRegularEnemies();
                moveSpecialEnemies();
                moveProjectiles();
                detectProjectileEnemyCollision();
                detectPlayerEnemyCollision();

                if (isWaveComplete()) {
                    waveIntermission = true;
                    waveIntermissionTimer = 4.0f;
                }
            }

            if (!player.active) gameState = GameState::GameOver;
        }

        BeginDrawing();

        ClearBackground(BLACK);

        if (gameState == GameState::Playing) {
            drawProjectiles();
            drawPlayer();
            drawRegularEnemies();
            drawSpecialEnemies();
            drawEnergyPickups();
            DrawText(TextFormat("HP: %d", player.HP), 20, 20, 24, WHITE);
            DrawText(TextFormat("Score: %u", player.score), 20, 50, 24, WHITE);
            DrawText(TextFormat("Wave: %d", currentWave), 20, 80, 24, WHITE);
            if (waveIntermission) {
                drawCenteredText("Wave cleared. Look alive, pilot. They're grouping up again.", 120,
                                 24, YELLOW);
                drawCenteredText(
                    TextFormat("Next wave in: %d", (int)std::ceil(waveIntermissionTimer)), 155, 24,
                    YELLOW);
            }
        } else if (gameState == GameState::Start) {
            drawCenteredText(GAME_NAME, SCREEN_HEIGHT / 2 - 60, 48, WHITE);
            drawCenteredText("Press ENTER to start", SCREEN_HEIGHT / 2 + 10, 24, LIGHTGRAY);
        } else {
            drawCenteredText("Game Over", SCREEN_HEIGHT / 2 - 60, 48, WHITE);
            drawCenteredText(TextFormat("Score: %u", player.score), SCREEN_HEIGHT / 2, 24,
                             LIGHTGRAY);
            drawCenteredText("Press ENTER to restart", SCREEN_HEIGHT / 2 + 40, 24, LIGHTGRAY);
        }

        EndDrawing();
    }

    UnloadTexture(player.texture);
    UnloadTexture(regularEnemyTexture);
    UnloadTexture(specialEnemyTexture);
    CloseWindow();

    return 0;
}
