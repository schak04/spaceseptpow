#include "player.hpp"

#include <cmath>

Player player;

// input -> acceleration -> velocity -> position
void movePlayer() {
    if (!player.active) return;

    const float dt = GetFrameTime();
    const float inputInducedAcc = 600.0f;

    Vector2 input = {0.0f, 0.0f};

    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) input.y -= 1.0f;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) input.y += 1.0f;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) input.x -= 1.0f;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) input.x += 1.0f;

    // check whether the first gamepad (index 0) is connected
    if (IsGamepadAvailable(0)) {
        // NOTE: ANALOG STICK INPUT
        // unlike keyboard keys, analog sticks return floating-point values
        // typically ranging from -1.0 to +1.0
        // -1.0 = fully left/up
        // 0.0 = centered
        // +1.0 = fully right/down
        // physical sticks might report small non-zero values even when untouched (stick drift)
        // -> a deadzone filters those out

        // check whether the controller has enough axes to include the left y axis
        // both the left x and y axes are needed to read the complete stick
        if (GetGamepadAxisCount(0) > GAMEPAD_AXIS_LEFT_Y) {
            // ignore stick movements whose absolute value is 0.2 or less
            const float deadzone = 0.2f;

            // read the horizontal (x) and vertical (y) axes of the left analog stick
            const float stickX = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
            const float stickY = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);

            // add the analog stick values to the input vector
            // add rather than assign because keyboard input might already exist
            if (std::fabs(stickX) > deadzone) input.x += stickX;
            if (std::fabs(stickY) > deadzone) input.y += stickY;
        }

        // NOTE: D-PAD INPUT
        // The D-pad is digital, just like a keyboard: each direction is either
        // pressed or not pressed. So it contributes -1 or +1 to the input.
        if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_UP)) input.y -= 1.0f;
        if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) input.y += 1.0f;
        if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) input.x -= 1.0f;
        if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) input.x += 1.0f;
    }

    // normalize input (if its magnitude is non-zero)
    // NOTE: magnitude of a vector, |v| = (x^2 + y^2)^(0.5)

    // normalize keyboard and d-pad input while preserving analog stick intensity
    float inputMagnitude = std::sqrt(input.x * input.x + input.y * input.y);
    if (inputMagnitude > 1.0f) {
        input.x /= inputMagnitude;
        input.y /= inputMagnitude;
    }

    player.acceleration = {0.0f, 0.0f};

    if (input.x != 0.0f || input.y != 0.0f) {
        player.acceleration.x = input.x * inputInducedAcc;
        player.acceleration.y = input.y * inputInducedAcc;
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

void drawPlayer() {
    if (!player.active) return;

    // NOTE: Rectangle -> x, y, width, height
    Rectangle srcRect = {0.0f, 0.0f, (float)player.texture.width, (float)player.texture.height};
    Rectangle destRect = {player.pos.x, player.pos.y, player.dimensions.x, player.dimensions.y};
    Vector2 origin = {0.0f, 0.0f};
    float rotation = 0.0f;
    DrawTexturePro(player.texture, srcRect, destRect, origin, rotation, WHITE);
}
