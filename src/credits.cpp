#include <raylib.h>

#include "constants.hpp"

namespace {

void drawCenteredText(const char* text, int y, int fontSize, Color color) {
    DrawText(text, (SCREEN_WIDTH - MeasureText(text, fontSize)) / 2, y, fontSize, color);
}

}

void drawCreditsScreen() {
    drawCenteredText("Credits", 90, 40, SKYBLUE);
    DrawRectangle(220, 155, 840, 2, DARKGRAY);

    drawCenteredText("Developed by", 190, 20, LIGHTGRAY);
    drawCenteredText("Saptaparno Chakraborty", 220, 30, GOLD);

    DrawText("ASSETS", 250, 300, 20, SKYBLUE);
    DrawText("Spaceships: gishadev, FREE Spaceships Vol. 1 (CC0 1.0)", 250, 335, 18,
             LIGHTGRAY);
    DrawText("Background: Space Parallax Background, OpenGameArt", 250, 365, 18, LIGHTGRAY);
    DrawText("Theme music: PhalanxKing", 250, 395, 18, LIGHTGRAY);
    DrawText("Regular wave music: TERMINEX", 250, 420, 18, LIGHTGRAY);
    DrawText("Boss fight music: Sirkoto51", 250, 445, 18, LIGHTGRAY);
    DrawText("Wave cleared music: BloodPixelHero", 250, 470, 18, LIGHTGRAY);
    DrawText("Game over music: AdamWeeden", 250, 495, 18, LIGHTGRAY);
    DrawText("Shooting sound: qubodup", 250, 520, 18, LIGHTGRAY);

    drawCenteredText("Asset links and full credits are in the project README.", 555, 18,
                     LIGHTGRAY);
    drawCenteredText("Press C, Y, ENTER, or START to return", 610, 20, GOLD);
}
