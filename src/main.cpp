#include <raylib.h>

#include <cmath>
#include <ctime>

#include "constants.hpp"
#include "credits.hpp"
#include "enemy.hpp"
#include "player-enemy-collision.hpp"
#include "player.hpp"
#include "projectile-enemy-collision.hpp"
#include "projectile.hpp"

Texture2D background;

enum class GameState { Start, Playing, GameOver, Credits };
enum class MusicTrack { None, Theme, RegularWave, BossFight, WaveCleared, GameOver };

void drawCenteredText(const char* text, int y, int fontSize, Color color) {
    DrawText(text, (SCREEN_WIDTH - MeasureText(text, fontSize)) / 2, y, fontSize, color);
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_NAME);
    SetTargetFPS(60);

    // audio
    InitAudioDevice();
    Music themeMusic = LoadMusicStream("assets/sounds/theme.mp3");
    Music regularWaveMusic = LoadMusicStream("assets/sounds/regular-wave.mp3");
    Music bossFightMusic = LoadMusicStream("assets/sounds/boss-fight.mp3");
    Music waveClearedMusic = LoadMusicStream("assets/sounds/wave-cleared.mp3");
    Music gameOverMusic = LoadMusicStream("assets/sounds/game-over.mp3");
    shootingSound = LoadSound("assets/sounds/shooting.mp3");
    collisionSound = LoadSound("assets/sounds/collision.mp3");
    waveClearedMusic.looping = false;
    gameOverMusic.looping = false;

    // background
    background = LoadTexture("assets/background/space-parallax.png");

    // change the starting state of the random number generator used by GetRandomValue()
    // so it's truly random
    SetRandomSeed((unsigned int)time(nullptr));

    // TODO: add error handling later sometime
    player.texture = LoadTexture("assets/player/player-spaceship.png");

    regularEnemyTexture = LoadTexture("assets/enemies/regular/regular-enemy-spaceship.png");
    specialEnemyTexture = LoadTexture("assets/enemies/special/special-enemy-spaceship.png");
    bossEnemyTexture = LoadTexture("assets/enemies/boss/boss-enemy-spaceship.png");

    GameState gameState = GameState::Start;
    int currentWave = 0;
    bool waveIntermission = false;
    float waveIntermissionTimer = 0.0f;
    bool paused = false;
    float escapeHoldTimer = 0.0f;
    bool shouldQuit = false;
    // to ensure the game over screen only shows after the game over music stops playing
    bool gameOverPending = false;
    // tracks active music state
    MusicTrack activeMusicTrack = MusicTrack::None;
    // gives access to the actual music object
    Music* activeMusic = nullptr;

    while (!shouldQuit) {
        // background drawing
        Rectangle backgroundSrcRect = {0.0f, 0.0f, (float)background.width,
                                       (float)background.height};
        Rectangle backgroundDestRect = {0.0f, 0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT};
        Vector2 backgroundOrigin = {0.0f, 0.0f};

        if (gameState == GameState::Playing)
            DrawTexturePro(background, backgroundSrcRect, backgroundDestRect, backgroundOrigin,
                           0.0f, WHITE);

        if (gameState == GameState::Playing && IsKeyPressed(KEY_ESCAPE)) paused = !paused;

        if (IsKeyDown(KEY_ESCAPE)) {
            escapeHoldTimer += GetFrameTime();
            if (escapeHoldTimer >= 1.5f) shouldQuit = true;
        } else {
            escapeHoldTimer = 0.0f;
        }

        if (gameState == GameState::Start && IsKeyPressed(KEY_ENTER)) {
            gameState = GameState::Playing;
            currentWave = 1;
            startWave(currentWave);
        } else if (gameState == GameState::Start && IsKeyPressed(KEY_C)) {
            gameState = GameState::Credits;
        } else if (gameState == GameState::Credits &&
                   (IsKeyPressed(KEY_C) || IsKeyPressed(KEY_ENTER))) {
            gameState = GameState::Start;
        } else if (gameState == GameState::GameOver && IsKeyPressed(KEY_ENTER)) {
            Texture2D playerTexture = player.texture;
            player = Player{};
            player.texture = playerTexture;
            resetEnemies();
            resetProjectiles();
            currentWave = 1;
            waveIntermission = false;
            startWave(currentWave);
            gameState = GameState::Playing;
        }

        if (gameState == GameState::Playing && !gameOverPending && !paused &&
            escapeHoldTimer == 0.0f && !shouldQuit) {
            // skipping gameplay updates to keep the cleared arena still
            // during the next-wave-starting warning
            if (waveIntermission) {
                waveIntermissionTimer -= GetFrameTime();
                if (waveIntermissionTimer <= 0.0f) {
                    waveIntermission = false;
                    currentWave++;
                    startWave(currentWave);
                }
            } else {
                movePlayer();
                spawnRegularEnemies();
                spawnSpecialEnemies();
                moveRegularEnemies();
                moveSpecialEnemies();
                updateBossEnemy();
                updateEnergyPickups();
                moveProjectiles();
                detectProjectileEnemyCollision();
                detectPlayerEnemyCollision();

                if (isWaveComplete()) {
                    waveIntermission = true;
                    waveIntermissionTimer = 4.0f;
                    StopMusicStream(regularWaveMusic);
                    StopMusicStream(bossFightMusic);
                    StopMusicStream(waveClearedMusic);
                    PlayMusicStream(waveClearedMusic);
                    activeMusic = &waveClearedMusic;
                    activeMusicTrack = MusicTrack::WaveCleared;
                }
            }

            if (!player.active) {
                gameOverPending = true;
                StopMusicStream(regularWaveMusic);
                StopMusicStream(bossFightMusic);
                StopMusicStream(waveClearedMusic);
                PlayMusicStream(gameOverMusic);
                activeMusic = &gameOverMusic;
                activeMusicTrack = MusicTrack::GameOver;
            }
        }

        if (gameState == GameState::Playing && !gameOverPending && !paused && !waveIntermission) {
            MusicTrack waveTrack =
                currentWave % 5 == 0 ? MusicTrack::BossFight : MusicTrack::RegularWave;
            if (activeMusicTrack != waveTrack) {
                if (activeMusic != nullptr) StopMusicStream(*activeMusic);
                activeMusic =
                    waveTrack == MusicTrack::BossFight ? &bossFightMusic : &regularWaveMusic;
                activeMusicTrack = waveTrack;
                PlayMusicStream(*activeMusic);
            }
        } else if (gameState == GameState::Playing && !gameOverPending && !paused &&
                   waveIntermission && activeMusicTrack != MusicTrack::WaveCleared) {
            if (activeMusic != nullptr) StopMusicStream(*activeMusic);
            activeMusic = &waveClearedMusic;
            activeMusicTrack = MusicTrack::WaveCleared;
            PlayMusicStream(waveClearedMusic);
        } else if (gameState == GameState::Start || gameState == GameState::GameOver ||
                   (gameState == GameState::Playing && paused)) {
            if (activeMusicTrack != MusicTrack::Theme) {
                if (activeMusic != nullptr) StopMusicStream(*activeMusic);
                activeMusic = &themeMusic;
                activeMusicTrack = MusicTrack::Theme;
                PlayMusicStream(themeMusic);
            }
        }

        if (activeMusic != nullptr) UpdateMusicStream(*activeMusic);

        // NOTE: tried checking
        // GetMusicTimePlayed(gameOverMusic) >= GetMusicTimeLength(gameOverMusic)
        // but it didn't work because GetMusicTimePlayed() wraps at the track length so it may never
        // reach or exceed GetMusicTimeLength()
        // IsMusicStreamPlaying(gameOverMusic) handles the same well
        if (gameOverPending && activeMusicTrack == MusicTrack::GameOver &&
            !IsMusicStreamPlaying(gameOverMusic)) {
            gameOverPending = false;
            gameState = GameState::GameOver;
        }

        BeginDrawing();

        ClearBackground(BLACK);

        if (gameState == GameState::Playing) {
            drawProjectiles();
            drawPlayer();
            drawRegularEnemies();
            drawSpecialEnemies();
            drawBossEnemy();
            drawEnergyPickups();
            DrawRectangle(20, 20, 200, 16, DARKGRAY);
            if (player.HP >= PLAYER_HP / 1.4) {
                DrawRectangle(20, 20, (int)(200.0f * player.HP / PLAYER_HP), 16, GREEN);
            } else if (player.HP >= PLAYER_HP / 2) {
                DrawRectangle(20, 20, (int)(200.0f * player.HP / PLAYER_HP), 16, ORANGE);
            } else {
                DrawRectangle(20, 20, (int)(200.0f * player.HP / PLAYER_HP), 16, RED);
            }
            DrawText(TextFormat("Score: %u", player.score), 20, 50, 24, WHITE);
            DrawText(TextFormat("Wave: %d", currentWave), 20, 80, 24, SKYBLUE);
            if (waveIntermission) {
                const char* intermissionMessage =
                    (currentWave + 1) % BOSS_FIGHT_WAVE == 0
                        ? "Wave cleared. Something big just got your address, pilot."
                        : "Wave cleared. Look alive, pilot. They're grouping up again.";
                drawCenteredText(intermissionMessage, 120, 24, YELLOW);
                drawCenteredText(
                    TextFormat("Next wave in: %d", (int)std::ceil(waveIntermissionTimer)), 155, 24,
                    BLUE);
            }
            if (paused) {
                drawCenteredText("Game Paused", (SCREEN_HEIGHT / 2) - 34, 32, YELLOW);
                drawCenteredText("Be back soon, pilot.", SCREEN_HEIGHT / 2, 28, BLUE);
            }
        } else if (gameState == GameState::Start) {
            drawCenteredText(GAME_NAME, SCREEN_HEIGHT / 2 - 60, 48, SKYBLUE);
            drawCenteredText("Press ENTER to start", SCREEN_HEIGHT / 2 + 10, 24, GOLD);
            drawCenteredText("Press C for credits", SCREEN_HEIGHT / 2 + 50, 20, LIGHTGRAY);
        } else if (gameState == GameState::Credits) {
            drawCreditsScreen();
        } else {
            drawCenteredText("Game Over", SCREEN_HEIGHT / 2 - 60, 48, ORANGE);
            drawCenteredText(TextFormat("Score: %u", player.score), SCREEN_HEIGHT / 2, 24,
                             LIGHTGRAY);
            drawCenteredText("Press ENTER to restart", SCREEN_HEIGHT / 2 + 40, 24, GOLD);
        }

        if (escapeHoldTimer > 0.0f) {
            const char* message = "Quitting game...";
            DrawText(message, SCREEN_WIDTH - MeasureText(message, 20) - 20, 20, 20, LIGHTGRAY);
        }

        EndDrawing();
    }

    UnloadTexture(player.texture);
    UnloadTexture(regularEnemyTexture);
    UnloadTexture(specialEnemyTexture);
    UnloadTexture(bossEnemyTexture);
    UnloadTexture(background);
    UnloadMusicStream(themeMusic);
    UnloadMusicStream(regularWaveMusic);
    UnloadMusicStream(bossFightMusic);
    UnloadMusicStream(waveClearedMusic);
    UnloadMusicStream(gameOverMusic);
    UnloadSound(shootingSound);
    UnloadSound(collisionSound);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
