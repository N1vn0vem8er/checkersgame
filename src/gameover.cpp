#include "gameover.h"
#include <raylib.h>

GameOver::GameOver() {}

State GameOver::update()
{
    if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ESCAPE))
        return State::MENU;
    return State::GAMEOVER;
}

void GameOver::draw()
{
    ClearBackground(RAYWHITE);

    float sw = GetScreenWidth();
    float sh = GetScreenHeight();

    DrawRectangle(0, 0, sw, sh, ColorAlpha(BLACK, 0.7f));

    int titleFontSize = sh * 0.07f;
    int subFontSize = sh * 0.03f;

    const char* winText = (winner == 1) ? "You won" : (winner == 2) ? "You lost" : "Draw";
    DrawText(winText, (sw - MeasureText(winText, titleFontSize)) / 2, sh * 0.38f, titleFontSize, (winner == 1) ? GREEN : (winner == 2) ? RED : YELLOW);

    const char* subText = "Press enter";
    DrawText(subText, (sw - MeasureText(subText, subFontSize)) / 2, sh * 0.52f, subFontSize, WHITE);
}
