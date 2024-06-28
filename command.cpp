#include "command.hpp"

Command* InputHandler::handleInput()
{
    if (isPressed()) execute();

    return NULL;
}

MoveMenuCommand::MoveMenuCommand(std::shared_ptr<Menu> menu, int x, int y)
{
    this->xBefore = menu->posX;
    this->yBefore = menu->posY;
    this->menu = menu;
    this->x = x;
    this->y = y;
}

MoveMenuCommand::execute()
{
    menu->moveMenu(x - xBefore, y - yBefore);
}

MoveMenuCommand::undo()
{
    menu->moveMenu(xBefore - x, yBefore - y);
}

InputHandler::InputHandler()
{
    // default key bindings
    keyMapping['w'] = buttonW;
    keyMapping['a'] = buttonA;
    keyMapping['s'] = buttonS;
    keyMapping['d'] = buttonD;
    keyMapping['e'] = buttonE;
    keyMapping['r'] = buttonR;
    keyMapping['x'] = buttonX;
    keyMapping['1'] = button1;
    keyMapping['2'] = button2;
    keyMapping['3'] = button3;
    keyMapping['4'] = button4;
    keyMapping['5'] = button5;
    keyMapping['6'] = button6;
    keyMapping['='] = buttonEqual;
    keyMapping['-'] = buttonMinus;
}
