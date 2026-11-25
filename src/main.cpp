#include <raylib.h>

#include <cmath>
#include <ctime>
#include <filesystem>
#include <string>

#include "constants.hpp"
#include "credits.hpp"
#include "enemy.hpp"
#include "keybinds.hpp"
#include "player-enemy-collision.hpp"
#include "player.hpp"
#include "projectile-enemy-collision.hpp"
#include "projectile.hpp"

Texture2D background;

enum class GameState { Start, Playing, GameOver, Credits, Keybinds };
enum class MusicTrack { None, Theme, RegularWave, BossFight, WaveCleared, GameOver };

void drawCenteredText(const char* text, int y, int fontSize, Color color) {
    DrawText(text, (SCREEN_WIDTH - MeasureText(text, fontSize)) / 2, y, fontSize, color);
}

// get the directory containing the running executable
// macOS stores resources inside the .app bundle, unlike other platforms
std::string getAssetPath(const char* relativePath) {
    std::filesystem::path assetDirectory = GetApplicationDirectory();
#ifdef __APPLE__
    assetDirectory /= "../Resources/assets";
#else
    assetDirectory /= "assets";
#endif
    return (assetDirectory / relativePath).string();
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_NAME);
    SetTargetFPS(60);

    // audio
    InitAudioDevice();
    Music themeMusic = LoadMusicStream(getAssetPath("sounds/theme.mp3").c_str());
    Music regularWaveMusic = LoadMusicStream(getAssetPath("sounds/regular-wave.mp3").c_str());
    Music bossFightMusic = LoadMusicStream(getAssetPath("sounds/boss-fight.mp3").c_str());
    Music waveClearedMusic = LoadMusicStream(getAssetPath("sounds/wave-cleared.mp3").c_str());
    Music gameOverMusic = LoadMusicStream(getAssetPath("sounds/game-over.mp3").c_str());
    shootingSound = LoadSound(getAssetPath("sounds/shooting.mp3").c_str());
    collisionSound = LoadSound(getAssetPath("sounds/collision.mp3").c_str());
    energyPickupSound = LoadSound(getAssetPath("sounds/energy-pickup.mp3").c_str());
    waveClearedMusic.looping = false;
    gameOverMusic.looping = false;

    // background
    background = LoadTexture(getAssetPath("background/space-parallax.png").c_str());
    RenderTexture2D gameScreen = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);

    // change the starting state of the random number generator used by GetRandomValue()
    // so it's truly random
    SetRandomSeed((unsigned int)time(nullptr));

    // TODO: add error handling later sometime
    player.texture = LoadTexture(getAssetPath("player/player-spaceship.png").c_str());

    regularEnemyTexture =
        LoadTexture(getAssetPath("enemies/regular/regular-enemy-spaceship.png").c_str());
    specialEnemyTexture =
        LoadTexture(getAssetPath("enemies/special/special-enemy-spaceship.png").c_str());
    bossEnemyTexture = LoadTexture(getAssetPath("enemies/boss/boss-enemy-spaceship.png").c_str());

    GameState gameState = GameState::Start;
    GameState menuReturnState = GameState::Start;
    int currentWave = 0;
    bool waveIntermission = false;
    float waveIntermissionTimer = 0.0f;
    bool paused = false;
    float quitHoldTimer = 0.0f;
    bool shouldQuit = false;
    // to ensure the game over screen only shows after the game over music stops playing
    bool gameOverPending = false;
    // tracks active music state
    MusicTrack activeMusicTrack = MusicTrack::None;
    // gives access to the actual music object
    Music* activeMusic = nullptr;

    while (!shouldQuit) {
        // check whether a controller is connected before querying its input
        // gamepad index 0 refers to the first connected controller
        const bool gamepadAvailable = IsGamepadAvailable(0);
        const bool gamepadStartPressed =
            gamepadAvailable && IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT);
        const bool gamepadStartDown =
            gamepadAvailable && IsGamepadButtonDown(0, GAMEPAD_BUTTON_MIDDLE_RIGHT);
        const bool gamepadFullscreenPressed =
            gamepadAvailable && IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_LEFT);
        const bool gamepadCreditsPressed =
            gamepadAvailable && IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_UP);
        const bool gamepadKeybindsPressed =
            gamepadAvailable && IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT);
        const bool gamepadBackPressed =
            gamepadAvailable && IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT);

        if (IsKeyPressed(KEY_F) || gamepadFullscreenPressed) ToggleBorderlessWindowed();

        if (gameState == GameState::Playing && (IsKeyPressed(KEY_ESCAPE) || gamepadStartPressed)) {
            paused = !paused;
        }

        if (IsKeyDown(KEY_ESCAPE) || gamepadStartDown) {
            quitHoldTimer += GetFrameTime();
            if (quitHoldTimer >= 1.5f) shouldQuit = true;
        } else {
            quitHoldTimer = 0.0f;
        }

        if (gameState == GameState::Start && (IsKeyPressed(KEY_ENTER) || gamepadStartPressed)) {
            gameState = GameState::Playing;
            currentWave = 1;
            startWave(currentWave);
        } else if (gameState == GameState::Start &&
                   (IsKeyPressed(KEY_C) || gamepadCreditsPressed)) {
            menuReturnState = GameState::Start;
            gameState = GameState::Credits;
        } else if (gameState == GameState::Start &&
                   (IsKeyPressed(KEY_K) || gamepadKeybindsPressed)) {
            menuReturnState = GameState::Start;
            gameState = GameState::Keybinds;
        } else if (gameState == GameState::Credits &&
                   (IsKeyPressed(KEY_C) || IsKeyPressed(KEY_ENTER) || gamepadStartPressed ||
                    gamepadCreditsPressed)) {
            gameState = menuReturnState;
        } else if (gameState == GameState::Keybinds &&
                   (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_K) || IsKeyPressed(KEY_ENTER) ||
                    gamepadBackPressed || gamepadKeybindsPressed || gamepadStartPressed)) {
            gameState = menuReturnState;
        } else if (gameState == GameState::GameOver &&
                   (IsKeyPressed(KEY_ENTER) || gamepadStartPressed)) {
            Texture2D playerTexture = player.texture;
            player = Player{};
            player.texture = playerTexture;
            resetEnemies();
            resetProjectiles();
            currentWave = 1;
            waveIntermission = false;
            startWave(currentWave);
            gameState = GameState::Playing;
        } else if (gameState == GameState::GameOver &&
                   (IsKeyPressed(KEY_C) || gamepadCreditsPressed)) {
            menuReturnState = GameState::GameOver;
            gameState = GameState::Credits;
        } else if (gameState == GameState::GameOver &&
                   (IsKeyPressed(KEY_K) || gamepadKeybindsPressed)) {
            menuReturnState = GameState::GameOver;
            gameState = GameState::Keybinds;
        }

        if (gameState == GameState::Playing && !gameOverPending && !paused &&
            quitHoldTimer == 0.0f && !shouldQuit) {
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
        BeginTextureMode(gameScreen);

        ClearBackground(BLACK);

        if (gameState == GameState::Playing) {
            Rectangle backgroundSrcRect = {0.0f, 0.0f, (float)background.width,
                                           (float)background.height};
            Rectangle backgroundDestRect = {0.0f, 0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT};
            DrawTexturePro(background, backgroundSrcRect, backgroundDestRect, {0.0f, 0.0f}, 0.0f,
                           WHITE);
        }

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
            drawCenteredText("Start: ENTER on keyboard or START on gamepad", SCREEN_HEIGHT / 2 + 20,
                             20, GOLD);
            drawCenteredText("Credits: C on keyboard or Y on gamepad", SCREEN_HEIGHT / 2 + 48, 20,
                             GREEN);
            drawCenteredText("View all keybinds: K on keyboard or X on gamepad",
                             SCREEN_HEIGHT / 2 + 76, 20, ORANGE);
        } else if (gameState == GameState::Credits) {
            drawCreditsScreen();
        } else if (gameState == GameState::Keybinds) {
            drawKeybindsScreen();
        } else {
            drawCenteredText("Game Over", SCREEN_HEIGHT / 2 - 60, 48, RED);
            drawCenteredText(TextFormat("Score: %u", player.score), SCREEN_HEIGHT / 2, 24,
                             LIGHTGRAY);
            drawCenteredText("Restart with ENTER on keyboard or START on gamepad",
                             SCREEN_HEIGHT / 2 + 40, 20, GOLD);
            drawCenteredText("Credits: C on keyboard or Y on gamepad", SCREEN_HEIGHT / 2 + 75, 18,
                             GREEN);
            drawCenteredText("Keybinds: K on keyboard or X on gamepad", SCREEN_HEIGHT / 2 + 105, 18,
                             SKYBLUE);
        }

        if (quitHoldTimer > 0.0f) {
            const char* message = "Quitting...";
            DrawText(message, SCREEN_WIDTH - MeasureText(message, 20) - 20, 20, 20, LIGHTGRAY);
        }

        EndTextureMode();

        ClearBackground(BLACK);
        const float scale =
            fminf((float)GetScreenWidth() / SCREEN_WIDTH, (float)GetScreenHeight() / SCREEN_HEIGHT);
        Rectangle gameScreenDest = {
            (GetScreenWidth() - SCREEN_WIDTH * scale) / 2.0f,
            (GetScreenHeight() - SCREEN_HEIGHT * scale) / 2.0f,
            SCREEN_WIDTH * scale,
            SCREEN_HEIGHT * scale,
        };
        Rectangle gameScreenSrc = {0.0f, 0.0f, (float)gameScreen.texture.width,
                                   -(float)gameScreen.texture.height};
        DrawTexturePro(gameScreen.texture, gameScreenSrc, gameScreenDest, {0.0f, 0.0f}, 0.0f,
                       WHITE);
        EndDrawing();
    }

    UnloadRenderTexture(gameScreen);
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
    UnloadSound(energyPickupSound);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
