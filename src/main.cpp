#include <raylib.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define GAME_NAME "SpaceSept Pow!"

struct Player {
    Vector2 pos = {(float)SCREEN_WIDTH * (1.0f / 2.0f), (float)SCREEN_HEIGHT * (94.0f / 100.0f)};
    Vector2 dimensions = {40, 40};
    Vector2 velocity = {0.0f, 0.0f};
    Vector2 acceleration = {0.0f, 0.0f};
};

Player player;

void movePlayer() {
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
        player.pos.y -= GetFrameTime() * player.velocity.y;
    }
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
        player.pos.y += GetFrameTime() * player.velocity.y;
    }
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        player.pos.x -= GetFrameTime() * player.velocity.x;
    }
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        player.pos.x += GetFrameTime() * player.velocity.x;
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

        DrawRectangleV(player.pos, player.dimensions, BLUE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
