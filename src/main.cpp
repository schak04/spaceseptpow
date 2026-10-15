#include <raylib.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define GAME_NAME "SpaceSept Pow!"

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_NAME);
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);
        DrawText(GAME_NAME, 32, 32, 32, RAYWHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
