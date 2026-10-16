#include <raylib.h>

#include <cmath>

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
    const float inputInducedAcc = 600.0f;

    Vector2 input = {0.0f, 0.0f};

    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) input.y -= 1.0f;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) input.y += 1.0f;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) input.x -= 1.0f;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) input.x += 1.0f;

    // normalize input (if its magnitude is non-zero)
    // NOTE: magnitude of a vector, |v| = (x^2 + y^2)^(0.5)
    float inputMagnitude = std::sqrt(input.x * input.x + input.y * input.y);

    player.acceleration = {0.0f, 0.0f};

    if (inputMagnitude > 0.0f) {
        player.acceleration.x = (input.x / inputMagnitude) * inputInducedAcc;
        player.acceleration.y = (input.y / inputMagnitude) * inputInducedAcc;
    }

    player.velocity.x += player.acceleration.x * dt;
    player.velocity.y += player.acceleration.y * dt;

    // NOTE: DRAG AND EXPONENTIAL DAMPING
    // Drag acceleration is proportional to velocity and opposite
    // in direction, so the faster it moves the faster it slows down.
    // dv/dt = -kv (k is the drag coeffcient or damping rate)
    // v(t) = v0 * e^(-kt) -> Exponential Decay
    // e^(-kt) is always positive for finite values of k and t
    if (player.acceleration.x == 0.0f && player.acceleration.y == 0) {
        const int drag = 2;
        const float damping = std::exp(-drag * dt);
        player.velocity.x *= damping;
        player.velocity.y *= damping;
    }

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
