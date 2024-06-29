#include "command.hpp"

Command* InputHandler::handleInput()
{
    if (isPressed()) execute();

    return NULL;
}

Command::Command(std::shared_ptr<Sim> sim)
{
    this->sim = sim;
}

MoveMenuCommand::MoveMenuCommand(std::shared_ptr<Sim> sim, int x, int y) : Command(sim)
{
    this->x = x;
    this->y = y;
}

void MoveMenuCommand::execute()
{
    for(auto menu : menus)
    {
        if(menu.second)
        {
            menu.first->moveMenu(x - menu.first->posX, y - menu.first->posY);
            return;
        }
    }
}

SwitchMenuCommand::SwitchMenuCommand()
{
}

void SwitchMenuCommand::execute()
{
}

ZoomCommand::ZoomCommand(std::shared_ptr<Sim> sim, int value) : Command(sim)
{
    this->scaleBefore = sim->scale;
    this->scale = value;
}

void ZoomCommand::execute()
{
    sim->scale *= this->scale;
}

void ZoomCommand::undo()
{
    sim->scale = this->scaleBefore;
}

QuitCommand::QuitCommand(std::shared_ptr<Sim> sim) : Command(sim)
{
}

void QuitCommand::execute()
{
}

bool Menu::isPressed(olc::Key c)
{
    return sim->GetKey(c).bHeld;
}

SelectElemCommand::SelectElemCommand(std::shared_ptr<Sim> sim, ElementType type) : Command(sim)
{
    this->type = type;
}

void SelectElemCommand::execute()
{
    sim->tempType = this->type;
}

InputHandler::InputHandler(std::shared_ptr<Sim> sim)
{
    this->sim = sim;
    
    
    buttonE = make_shared<SwitchMenuCommand>(sim);
    buttonR = make_shared<RunSimulationCommand>(sim);
    buttonX = make_shared<QuitCommand>(sim);

    buttonEqual = make_shared<ZoomCommand>(sim, 1.001f);
    buttonMinus = make_shared<ZoomCommand>(sim, 0.999f);
    
    // default key bindings
    keyMapping[olc::W] = buttonW;
    keyMapping[olc::A] = buttonA;
    keyMapping[olc::S] = buttonS;
    keyMapping[olc::D] = buttonD;
    keyMapping[olc::E] = buttonE;
    keyMapping[olc::R] = buttonR;
    keyMapping[olc::X] = buttonX;
    keyMapping[olc::K1] = button1;
    keyMapping[olc::K2] = button2;
    keyMapping[olc::K3] = button3;
    keyMapping[olc::K4] = button4;
    keyMapping[olc::K5] = button5;
    keyMapping[olc::EQUALS] = buttonEqual;
    keyMapping[olc::MINUS] = buttonMinus;
}

std::shared_ptr<Command> InputHandler::handleInput()
{
    if(sim->menus[0].second)// main menu activated
    {
        button1 = nullptr;
        button2 = nullptr;
        button3 = nullptr;
        button4 = nullptr;
        button5 = nullptr;
    }
    else if(sim->menus[1].second) // edit menu activated
    {
        button1 = std::make_shared<SwitchMenu>(sim, 1, 3);
        button2 = std::make_shared<SwitchMenu>(sim, 1, 4);
        button3 = std::make_shared<SwitchMenu>(sim, 1, 2);
        button4 = nullptr;
        button5 = nullptr;
    }
    else if(sim->menus[2].second) // modify menu activated
    {
        button1 = nullptr;
        button2 = nullptr;
        button3 = nullptr;
        button4 = nullptr;
        button5 = nullptr; 
    }
    else if(sim->menus[3].second) // add menu activated
    {
        button1 = std::make_shared<SelectElemCommand>(sim, ElementType::ELEM_CABLE);
        button2 = std::make_shared<SelectElemCommand>(sim, ElementType::ELEM_RESISTOR);
        button3 = std::make_shared<SelectElemCommand>(sim, ElementType::ELEM_TRANSISTOR);
        button4 = std::make_shared<SelectElemCommand>(sim, ElementType::ELEM_SOURCE);
        button5 = std::make_shared<SelectElemCommand>(sim, ElementType::ELEM_BATTERY);
    }
    else if(sim->menus[4].second) // delete menu activated
    {   
        button1 = nullptr;
        button2 = nullptr;
        button3 = nullptr;
        button4 = nullptr;
        button5 = nullptr; 
    }
    else // no menu is active
    {   
        button1 = nullptr;
        button2 = nullptr;
        button3 = nullptr;
        button4 = nullptr;
        button5 = nullptr;
    }

    for(auto entry : keyMapping)
    {
        if(isPressed(entry.first))
            if(entry.second != nullptr) return entry.second;
    }

    if(isPressed(olc::ESCAPE)) // cannot remap ESC
    {
        for(auto entry : sim->menus)
        {
            entry.second = false;
        }

        sim->menus[0].second = true;
    }
}
