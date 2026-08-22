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
    Rectangle startButtonBounds {250, 350, 300, 60};
    Rectangle exitButtonBounds {250, 450, 300, 60};
};

#endif // MENU_H
