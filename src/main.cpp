#include <raylib.h>

int main() {
    InitWindow(1280, 720, "SpaceSept Pow!");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);
        DrawText("SpaceSept Pow!", 32, 32, 32, RAYWHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
