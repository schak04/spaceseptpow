#include <raylib.h>

#include <string>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define GAME_NAME "SpaceSept Pow!"

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_NAME);
    SetTargetFPS(60);

    Vector2 circPos = {(float)SCREEN_WIDTH * (1.0f / 60.0f), (float)SCREEN_HEIGHT / 3.0f};
    const float circRad = 20.0f;
    const float speed = 200.0f;

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);
        DrawText(GAME_NAME, (float)SCREEN_HEIGHT * (1.0f / 80.0f),
                 (float)SCREEN_WIDTH * (1.0f / 40.0f), 32, RAYWHITE);
        DrawText(std::to_string(GetFPS()).c_str(), (float)SCREEN_HEIGHT * (1.0f / 80.0f),
                 (float)SCREEN_WIDTH * (2.0f / 40.0f), 32, GREEN);

        circPos.x += GetFrameTime() * speed;

        DrawCircleV(circPos, circRad, BLUE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
