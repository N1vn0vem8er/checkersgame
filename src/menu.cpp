#include "menu.h"

Menu::Menu() {}

State Menu::update()
{
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

    DrawText("Checkers", 270, 180, 60, DARKGRAY);

    Vector2 mousePos = GetMousePosition();
    DrawRectangleRec(startButtonBounds, CheckCollisionPointRec(mousePos, startButtonBounds) ? LIGHTGRAY : GRAY);
    DrawRectangleLinesEx(startButtonBounds, 3, DARKGRAY);
    DrawText("New game", startButtonBounds.x + 65, startButtonBounds.y + 15, 30, WHITE);

    DrawRectangleRec(exitButtonBounds, CheckCollisionPointRec(mousePos, exitButtonBounds) ? LIGHTGRAY : GRAY);
    DrawRectangleLinesEx(exitButtonBounds, 3, DARKGRAY);
    DrawText("Exit", exitButtonBounds.x + 85, exitButtonBounds.y + 15, 30, WHITE);
}
