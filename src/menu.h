#ifndef MENU_H
#define MENU_H

#include "state.h"
#include <raylib.h>

class Menu
{
public:
    Menu();
    State update();
    void draw();
    bool vsAi {true};

private:
    Rectangle startAiButtonBounds;
    Rectangle startHumanButtonBounds;
    Rectangle exitButtonBounds;
};

#endif // MENU_H
