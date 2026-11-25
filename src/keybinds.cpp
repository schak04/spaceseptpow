#include <raylib.h>

#include "constants.hpp"

namespace {

void drawCenteredText(const char* text, int y, int fontSize, Color color) {
    DrawText(text, (SCREEN_WIDTH - MeasureText(text, fontSize)) / 2, y, fontSize, color);
}

}  // namespace

void drawKeybindsScreen() {
    drawCenteredText("Keybinds", 45, 36, SKYBLUE);
    drawCenteredText("Keyboard", 105, 22, GOLD);
    drawCenteredText("Move: WASD or Arrow Keys", 135, 18, LIGHTGRAY);
    drawCenteredText("Shoot: Space", 157, 18, LIGHTGRAY);
    drawCenteredText("Start or restart: Enter", 179, 18, LIGHTGRAY);
    drawCenteredText("Pause or resume: Escape", 201, 18, LIGHTGRAY);
    drawCenteredText("Quit: Hold Escape for 1.5 seconds", 223, 18, LIGHTGRAY);
    drawCenteredText("Credits: C", 245, 18, LIGHTGRAY);
    drawCenteredText("View keybinds: K", 267, 18, LIGHTGRAY);
    drawCenteredText("Toggle fullscreen: F", 289, 18, LIGHTGRAY);
    drawCenteredText("Gamepad", 340, 22, GOLD);
    drawCenteredText("Move: Left Stick or D-pad", 370, 18, LIGHTGRAY);
    drawCenteredText("Shoot: A", 392, 18, LIGHTGRAY);
    drawCenteredText("Start or restart: Start / Menu", 414, 18, LIGHTGRAY);
    drawCenteredText("Pause or resume: Start / Menu", 436, 18, LIGHTGRAY);
    drawCenteredText("Quit: Hold Start / Menu for 1.5 seconds", 458, 18, LIGHTGRAY);
    drawCenteredText("Credits: Y", 480, 18, LIGHTGRAY);
    drawCenteredText("View keybinds: X", 502, 18, LIGHTGRAY);
    drawCenteredText("Toggle fullscreen: View / Select", 524, 18, LIGHTGRAY);
    drawCenteredText("Return to previous screen:", 585, 18, GOLD);
    drawCenteredText("Escape, K, or Enter on keyboard", 610, 18, LIGHTGRAY);
    drawCenteredText("B, X, or Start / Menu on gamepad", 635, 18, LIGHTGRAY);
}
