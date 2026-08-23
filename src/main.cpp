#include "board.h"
#include "gameover.h"
#include "menu.h"
#include "state.h"
#include <raylib.h>

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1200, 800, "Checkers");
    State state = State::MENU;
    Menu menu;
    Board board(menu.vsAi);
    GameOver gameover;
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
        case State::GAMEOVER:
            gameover.winner = board.winner;
            state = gameover.update();
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
        case State::GAMEOVER:
            gameover.draw();
            break;
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
