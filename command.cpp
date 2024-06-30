#include "command.hpp"

MoveMenuCommand::MoveMenuCommand(int x, int y) 
{
    this->x = x;
    this->y = y;
}
// cppcheck-suppress unusedFunction
void MoveMenuCommand::execute(Sim* sim)
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

SwitchMenuCommand::SwitchMenuCommand(int menuIndex1, int menuIndex2) 
{
    this->menuIndex1 = menuIndex1;
    this->menuIndex2 = menuIndex2;
}

void SwitchMenuCommand::execute(Sim* sim)
{
}

ZoomCommand::ZoomCommand(float value) 
{
    this->scale = value;
}

void ZoomCommand::execute(Sim* sim)
{
    sim->setScale(sim->getScale() * this->scale);
}

void QuitCommand::execute(Sim* sim)
{
}

SelectElemCommand::SelectElemCommand(ElementType type) 
{
    this->type = type;
}

void SelectElemCommand::execute(Sim* sim)
{
    sim->setTempType(this->type);
}

void RunSimulationCommand::execute(Sim* sim)
{
    sim->getCircuit()->run();
}

InputHandler::InputHandler()
{
    buttonR = std::make_shared<RunSimulationCommand>();
    buttonX = std::make_shared<QuitCommand>();

    buttonEqual = std::make_shared<ZoomCommand>(1.001f);
    buttonMinus = std::make_shared<ZoomCommand>(0.999f);

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
std::shared_ptr<Command> InputHandler::handleInput(Sim* sim, olc::PixelGameEngine* pge)
{

    for(long unsigned int i = 0; i < (sim->getMenus()).size(); i++)
    {
        if((sim->getMenus())[i].second)
            buttonE = std::make_shared<SwitchMenuCommand>(i,1);
    }
    
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
        button1 = std::make_shared<SwitchMenuCommand>(1, 3);
        button2 = std::make_shared<SwitchMenuCommand>(1, 4);
        button3 = std::make_shared<SwitchMenuCommand>(1, 2);
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
        button1 = std::make_shared<SelectElemCommand>(ElementType::ELEM_CABLE);
        button2 = std::make_shared<SelectElemCommand>(ElementType::ELEM_RESISTOR);
        button3 = std::make_shared<SelectElemCommand>(ElementType::ELEM_TRANSISTOR);
        button4 = std::make_shared<SelectElemCommand>(ElementType::ELEM_SOURCE);
        button5 = std::make_shared<SelectElemCommand>(ElementType::ELEM_BATTERY);
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
        if(pge->GetKey(entry.first).bPressed)
            if(entry.second != nullptr) return entry.second;
    }

    if(pge->GetKey(olc::ESCAPE).bPressed) // cannot remap ESC
    {
        for(auto entry : sim->getMenus())
        {
            entry.second = false;
        }

        (sim->getMenus())[0].second = !(sim->getMenus())[0].second;
    }

    return NULL;
}

InputHandler::~InputHandler()
{
}

/*
void Engine::mouseControls()
{
    olc::vf2d mousePos = {(float)GetMouseX(), (float)GetMouseY() };

    if(GetMouse(1).bPressed)
    {
        startPan = mousePos;
    }

    if(GetMouse(1).bHeld)
    {
        worldOffset -= (mousePos - startPan) / sim->getScale(); 
        startPan = mousePos;
    }

    olc::vf2d mousePosBZoom;
    ScreenToWorld((int)mousePos.x, (int)mousePos.y, mousePosBZoom);

    if(GetMouseWheel() > 0)
    {
        if(sim->getScale() < 100.0f)
            sim->setScale(sim->getScale() * 1.1f);
        else
            sim->setScale(100.0f);
    }
    if(GetMouseWheel() < 0)
    {
        if(sim->getScale() > 10.0f)
            sim->setScale(sim->getScale() * 0.9f);
        else
            sim->setScale(10.0f);
    }
    olc::vf2d mousePosAZoom;
    ScreenToWorld((int)mousePos.x, (int)mousePos.y, mousePosAZoom);
    worldOffset += (mousePosBZoom - mousePosAZoom);

}
*/
