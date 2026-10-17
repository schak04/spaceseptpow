#include <raylib.h>

#include <cmath>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define GAME_NAME "SpaceSept Pow!"

struct Player {
    Texture2D texture;
    Vector2 pos = {(float)SCREEN_WIDTH * (1.0f / 2.0f), (float)SCREEN_HEIGHT * (94.0f / 100.0f)};
    Vector2 dimensions = {80, 80};
    Vector2 velocity = {0.0f, 0.0f};
    Vector2 acceleration = {0.0f, 0.0f};
};

Player player;

#define MAX_PROJECTILES 100

struct Projectile {
    Vector2 pos = {player.pos.x + (player.dimensions.x / 2.0f), player.pos.y};
    Vector2 velocity = {0.0f, 0.0f};
    float radius = 4.0f;
    bool active = false;
};

Projectile projectiles[MAX_PROJECTILES];

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

    // horizontal clamping
    if (player.pos.x < 0.0f) {
        player.pos.x = 0.0f;
        player.velocity.x = 0.0f;
    } else if (player.pos.x > (float)SCREEN_WIDTH - player.dimensions.x) {
        player.pos.x = (float)SCREEN_WIDTH - player.dimensions.x;
        player.velocity.x = 0.0f;
    }

    // vertical clamping
    if (player.pos.y < 0.0f) {
        player.pos.y = 0.0f;
        player.velocity.y = 0.0f;
    } else if (player.pos.y > (float)SCREEN_HEIGHT - player.dimensions.y) {
        player.pos.y = (float)SCREEN_HEIGHT - player.dimensions.y;
        player.velocity.y = 0.0f;
    }
}

const float projectileSpeed = 2000.0f;
void updateProjectiles() {
    const float dt = GetFrameTime();

    // find a free slot in the projectiles array to spawn a projectile
    // and skip firing if all slots are occupied
    if (IsKeyPressed(KEY_SPACE)) {
        for (int i = 0; i < MAX_PROJECTILES; i++) {
            if (!projectiles[i].active) {
                // projectile spawning
                projectiles[i].pos = {player.pos.x + (player.dimensions.x / 2.0f), player.pos.y};
                projectiles[i].velocity = {0.0f, -projectileSpeed};
                projectiles[i].active = true;
                break;
            }
        }
    }

    // update active projectiles and remove when out of screen
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!projectiles[i].active) continue;

        projectiles[i].pos.x += projectiles[i].velocity.x * dt;
        projectiles[i].pos.y += projectiles[i].velocity.y * dt;

        // remove projectile if its
        // bottom edge crosses the game's upper boundary
        // or top edge -> lower boundary
        // or right edge -> left boundary
        // or left edge -> right boundary
        if (projectiles[i].pos.y + projectiles[i].radius < 0.0f ||
            projectiles[i].pos.y - projectiles[i].radius > (float)SCREEN_HEIGHT ||
            projectiles[i].pos.x + projectiles[i].radius < 0.0f ||
            projectiles[i].pos.x - projectiles[i].radius > (float)SCREEN_WIDTH) {
            projectiles[i].active = false;
        }
    }
}

void drawProjectiles() {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (projectiles[i].active) {
            DrawCircleV(projectiles[i].pos, projectiles[i].radius, GOLD);
        }
    }
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_NAME);
    SetTargetFPS(60);

    // TODO: add error handling later sometime
    player.texture = LoadTexture("assets/player/player-spaceship-blue.png");

    while (!WindowShouldClose()) {
        movePlayer();
        updateProjectiles();

        BeginDrawing();

        ClearBackground(BLACK);

        drawProjectiles();

        // NOTE: Rectangle -> x, y, width, height
        Rectangle srcRect = {0.0f, 0.0f, (float)player.texture.width, (float)player.texture.height};
        Rectangle destRect = {player.pos.x, player.pos.y, player.dimensions.x, player.dimensions.y};
        Vector2 origin = {0.0f, 0.0f};
        float rotation = 0.0f;
        DrawTexturePro(player.texture, srcRect, destRect, origin, rotation, WHITE);

        EndDrawing();
    }

    UnloadTexture(player.texture);
    CloseWindow();

    return 0;
}
