#include <raylib.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define GAME_NAME "SpaceSept Pow!"

struct Player {
    Vector2 playerPosition = {(float)SCREEN_WIDTH * (1.0f / 2.0f),
                              (float)SCREEN_HEIGHT * (94.0f / 100.0f)};
    Vector2 playerDimensions = {40, 40};
    const float playerSpeedVertical = 225.0f;
    const float playerSpeedHorizontal = 400.0f;
};

Player player;

void movePlayer() {
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
        player.playerPosition.y -= GetFrameTime() * player.playerSpeedVertical;
    }
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
        player.playerPosition.y += GetFrameTime() * player.playerSpeedVertical;
    }
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        player.playerPosition.x -= GetFrameTime() * player.playerSpeedHorizontal;
    }
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        player.playerPosition.x += GetFrameTime() * player.playerSpeedHorizontal;
    }
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_NAME);
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        movePlayer();

        BeginDrawing();

        ClearBackground(BLACK);
        DrawText(GAME_NAME, (float)SCREEN_HEIGHT * (1.0f / 80.0f),
                 (float)SCREEN_WIDTH * (1.0f / 40.0f), 32, RAYWHITE);

        DrawRectangleV(player.playerPosition, player.playerDimensions, BLUE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
