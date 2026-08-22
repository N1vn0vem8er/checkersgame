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

private:
    Rectangle startButtonBounds;
    Rectangle exitButtonBounds;
};

#endif // MENU_H
