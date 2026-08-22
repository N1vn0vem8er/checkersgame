#include "menu.h"

Menu::Menu() {}

State Menu::update()
{
    float sw = GetScreenWidth();
    float sh = GetScreenHeight();
    float btnWidth = sw * 0.35f;
    float btnHeight = sh * 0.08f;
    float btnX = (sw - btnWidth) / 2.0f;
    startButtonBounds = { btnX, sh * 0.45f, btnWidth, btnHeight };
    exitButtonBounds = { btnX, sh * 0.58f, btnWidth, btnHeight };
    Vector2 mousePos = GetMousePosition();

    if(CheckCollisionPointRec(mousePos, startButtonBounds))
    {
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            return State::GAME;
    }

    if(CheckCollisionPointRec(mousePos, exitButtonBounds))
    {
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            return State::EXIT;
    }
    return State::MENU;
}

void Menu::draw()
{
    ClearBackground(RAYWHITE);

    float sw = GetScreenWidth();
    float sh = GetScreenHeight();
    int titleFontSize = sh * 0.08f;
    int btnFontSize = sh * 0.035f;

    const char* title = "Checkers";
    DrawText(title, (sw - MeasureText(title, titleFontSize)) / 2, sh * 0.2f, titleFontSize, DARKGRAY);

    Vector2 mousePos = GetMousePosition();

    DrawRectangleRec(startButtonBounds, CheckCollisionPointRec(mousePos, startButtonBounds) ? LIGHTGRAY : GRAY);
    DrawRectangleLinesEx(startButtonBounds, 3, DARKGRAY);

    const char* startText = "New game";
    DrawText(startText, startButtonBounds.x + (startButtonBounds.width - MeasureText(startText, btnFontSize)) / 2, startButtonBounds.y + (startButtonBounds.height - btnFontSize) / 2, btnFontSize, WHITE);
    DrawRectangleRec(exitButtonBounds, CheckCollisionPointRec(mousePos, exitButtonBounds) ? LIGHTGRAY : GRAY);
    DrawRectangleLinesEx(exitButtonBounds, 3, DARKGRAY);

    const char* exitText = "Exit";
    DrawText(exitText, exitButtonBounds.x + (exitButtonBounds.width - MeasureText(exitText, btnFontSize)) / 2, exitButtonBounds.y + (exitButtonBounds.height - btnFontSize) / 2, btnFontSize, WHITE);
}
