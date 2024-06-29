#include "command.hpp"

Command::Command(std::shared_ptr<Sim> sim)
{
    this->sim = sim;
}

MoveMenuCommand::MoveMenuCommand(std::shared_ptr<Sim> sim, int x, int y) : Command(sim)
{
    this->x = x;
    this->y = y;
}
// cppcheck-suppress unusedFunction
void MoveMenuCommand::execute()
{

    for(auto menu : sim->getMenus())
    {
        if(menu.second)
        {
            menu.first->moveMenu(x - menu.first->getPosX(), y - menu.first->getPosY());
            return;
        }
    }
}

SwitchMenuCommand::SwitchMenuCommand(std::shared_ptr<Sim> sim, int menuIndex1, int menuIndex2) : Command(sim)
{
    this->menuIndex1 = menuIndex1;
    this->menuIndex2 = menuIndex2;
}

void SwitchMenuCommand::execute()
{
}

ZoomCommand::ZoomCommand(std::shared_ptr<Sim> sim, float value) : Command(sim)
{
    this->scaleBefore = sim->getScale();
    this->scale = value;
}

void ZoomCommand::execute()
{
    sim->setScale(sim->getScale() * this->scale);
}
// cppcheck-suppress unusedFunction
void ZoomCommand::undo()
{
    sim->setScale(this->scaleBefore);
}

QuitCommand::QuitCommand(std::shared_ptr<Sim> sim) : Command(sim)
{
}

void QuitCommand::execute()
{
}

bool InputHandler::isPressed(olc::Key c)
{
    return sim->GetKey(c).bHeld;
}

SelectElemCommand::SelectElemCommand(std::shared_ptr<Sim> sim, ElementType type) : Command(sim)
{
    this->type = type;
}

void SelectElemCommand::execute()
{
    sim->setTempType(this->type);
}

RunSimulationCommand::RunSimulationCommand(std::shared_ptr<Sim> sim) : Command(sim)
{
}

void RunSimulationCommand::execute()
{
    this->sim->getCircuit()->run();
}

InputHandler::InputHandler(std::shared_ptr<Sim> sim)
{
    this->sim = sim;
    
    for(long unsigned int i = 0; i < (sim->getMenus()).size(); i++)
    {
        if((sim->getMenus())[i].second)
            buttonE = make_shared<SwitchMenuCommand>(sim,i,1);
    }
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
// cppcheck-suppress unusedFunction
std::shared_ptr<Command> InputHandler::handleInput()
{
    if((sim->getMenus())[0].second)// main menu activated
    {
        button1 = nullptr;
        button2 = nullptr;
        button3 = nullptr;
        button4 = nullptr;
        button5 = nullptr;
    }
    else if((sim->getMenus())[1].second) // edit menu activated
    {
        button1 = std::make_shared<SwitchMenuCommand>(sim, 1, 3);
        button2 = std::make_shared<SwitchMenuCommand>(sim, 1, 4);
        button3 = std::make_shared<SwitchMenuCommand>(sim, 1, 2);
        button4 = nullptr;
        button5 = nullptr;
    }
    else if((sim->getMenus())[2].second) // modify menu activated
    {
        button1 = nullptr;
        button2 = nullptr;
        button3 = nullptr;
        button4 = nullptr;
        button5 = nullptr; 
    }
    else if((sim->getMenus())[3].second) // add menu activated
    {
        button1 = std::make_shared<SelectElemCommand>(sim, ElementType::ELEM_CABLE);
        button2 = std::make_shared<SelectElemCommand>(sim, ElementType::ELEM_RESISTOR);
        button3 = std::make_shared<SelectElemCommand>(sim, ElementType::ELEM_TRANSISTOR);
        button4 = std::make_shared<SelectElemCommand>(sim, ElementType::ELEM_SOURCE);
        button5 = std::make_shared<SelectElemCommand>(sim, ElementType::ELEM_BATTERY);
    }
    else if((sim->getMenus())[4].second) // delete menu activated
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
        for(auto entry : sim->getMenus())
        {
            entry.second = false;
        }

        (sim->getMenus())[0].second = true;
    }

    return NULL;
}
