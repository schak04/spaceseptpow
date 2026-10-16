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

// input -> acceleration -> velocity -> position
void movePlayer() {
    const float dt = GetFrameTime();
    const float inputInducedAcc = 200.0f;

    player.acceleration = {0.0f, 0.0f};

    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
        player.acceleration.y -= inputInducedAcc;
    }
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
        player.acceleration.y += inputInducedAcc;
    }
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        player.acceleration.x -= inputInducedAcc;
    }
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        player.acceleration.x += inputInducedAcc;
    }

    player.velocity.x += player.acceleration.x * dt;
    player.velocity.y += player.acceleration.y * dt;

    player.pos.x += player.velocity.x * dt;
    player.pos.y += player.velocity.y * dt;
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
