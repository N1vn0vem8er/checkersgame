#ifndef GAMEOVER_H
#define GAMEOVER_H

#include "state.h"
class GameOver
{
public:
    GameOver();
    State update();
    void draw();
    int winner {0};

};

#endif // GAMEOVER_H
