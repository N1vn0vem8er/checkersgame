#include "board.h"
#include "state.h"
#include <raylib.h>

int main()
{
    InitWindow(1200, 800, "Checkers");
    State state = State::GAME;
    Board board;
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
    while(!WindowShouldClose() && state != State::EXIT)
    {
        switch(state)
        {
        case State::EXIT:
            break;
        case State::MENU:
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
