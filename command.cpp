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

void MoveMenuCommand::execute()
{
    menu->moveMenu(x - xBefore, y - yBefore);
}

SwitchMenuCommand::SwitchMenuCommand()
{
}

void SwitchMenuCommand::execute()
{
}

ZoomCommand::ZoomCommand(std::shared_ptr<Sim> sim, int value)
{
    this->sim = sim;
    this->scaleBefore = sim->scale;
    this->scale = value;
}

void ZoomCommand::execute()
{
}

void ZoomCommand::undo()
{
}

QuitCommand::QuitCommand(std::shared_ptr<Sim> sim)
{
    this->sim = sim;
}

void QuitCommand::execute()
{
}

bool Menu::isPressed(char c)
{

}

SelectElemCommand::SelectElemCommand(std::shared_ptr<Sim> sim, ElementType type)
{
    this->sim = sim;
    this->type = type;
}

void SelectElemCommand::execute()
{
    sim->tempType = this->type;
}

InputHandler::InputHandler(std::shared_ptr<Sim> sim)
{
    this->sim = sim;

    buttonW = make_shared<MoveMenuCommand>(sim, 0, -1);
    buttonA = make_shared<MoveMenuCommand>(sim, 0, -1);
    buttonS = make_shared<MoveMenuCommand>(sim, 0,  1);
    buttonD = make_shared<MoveMenuCommand>(sim, 0,  1);

    buttonE = make_shared<SwitchMenuCommand>(sim);
    buttonR = make_shared<RunSimulationCommand>(sim);
    buttonX = make_shared<QuitCommand>(sim);

    buttonEqual = make_shared<ZoomCommand>(sim, 1.001f);
    buttonMinus = make_shared<ZoomCommand>(sim, 0.999f);
    
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

std::shared_ptr<Command> handleInput()
{
    if(sim->menus[0].first == true) // main menu activated
    {
    }
    else if(sim->menus[1].first == true) // edit menu activated
    {
        button1 = std::make_shared<SwitchMenu>(sim, 1, 3);
        button2 = std::make_shared<SwitchMenu>(sim, 1, 4);
        button3 = std::make_shared<SwitchMenu>(sim, 1, 2);
    }
    else if(sim->menus[2].first) // modify menu activated
    {
        button1 = std::make_shared<>;
        button2 = std::make_shared<>;
        button3 = std::make_shared<>;
        button4 = std::make_shared<>;
        button5 = std::make_shared<>;
        button6 = std::make_shared<>;
    }
    else if(sim->menus[3].first) // add menu activated
    {
        button1 = std::make_shared<>;
        button2 = std::make_shared<>;
        button3 = std::make_shared<>;
        button4 = std::make_shared<>;
        button5 = std::make_shared<>;
        button6 = std::make_shared<>;
    }
    else if(sim->menus[4].first) // delete menu activated
    {
        button1 = std::make_shared<>;
        button2 = std::make_shared<>;
        button3 = std::make_shared<>;
        button4 = std::make_shared<>;
        button5 = std::make_shared<>;
        button6 = std::make_shared<>;
    }
    else // no menu is active
    {
        button1 = nullptr;
        button2 = nullptr;
        button3 = nullptr;
        button4 = nullptr;
        button5 = nullptr;
        button6 = nullptr;
    }

    for(auto entry : keyMapping)
    {
        if(isPressed(entry.first))
            if(entry.second != nullptr) return entry.second;
    }
}
