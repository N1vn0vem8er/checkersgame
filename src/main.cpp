#include "board.h"
#include "menu.h"
#include "state.h"
#include <raylib.h>

int main()
{
    InitWindow(1200, 800, "Checkers");
    State state = State::MENU;
    Board board;
    Menu menu;
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
    while(!WindowShouldClose() && state != State::EXIT)
    {
        switch(state)
        {
        case State::EXIT:
            break;
        case State::MENU:
            state = menu.update();
            break;
        case State::GAME:
            state = board.update();
            break;
        }

        switch(state)
        {
        case State::EXIT:
            break;
        case State::MENU:
            menu.draw();
            break;
        case State::GAME:
            board.draw();
            break;
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
