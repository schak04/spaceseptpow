#include <raylib.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define GAME_NAME "SpaceSept Pow!"

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_NAME);
    SetTargetFPS(60);

    Vector2 playerPosition = {(float)SCREEN_WIDTH * (1.0f / 2.0f),
                              (float)SCREEN_HEIGHT * (94.0f / 100.0f)};
    Vector2 playerDimensions = {40, 40};

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);
        DrawText(GAME_NAME, (float)SCREEN_HEIGHT * (1.0f / 80.0f),
                 (float)SCREEN_WIDTH * (1.0f / 40.0f), 32, RAYWHITE);

        DrawRectangleV(playerPosition, playerDimensions, BLUE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
