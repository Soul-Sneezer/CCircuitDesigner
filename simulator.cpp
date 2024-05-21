#include "simulator.hpp"

    OperationFailed::OperationFailed(const char* message) throw() : std::runtime_error(message)
    {

    }

    void Sim::resetOffset()
    {
        menuOffset = {0,0};
    }

    void Sim::pressEntry(const olc::vi2d pos, const std::string& key, const std::string& action)
    {
        DrawString(pos.x     , pos.y, "Press ");
        DrawString(pos.x + 45, pos.y, key, olc::GREEN);
        DrawString(pos.x + 50, pos.y, " to ");
        DrawString(pos.x + 80, pos.y, action);
    }
    void Sim::pressEntry(const olc::vi2d pos, const std::string& key1, const std::string& key2, const std::string& action)
    {
        DrawString(pos.x      , pos.y, "Press ");
        DrawString(pos.x + 45 , pos.y, key1, olc::GREEN);
        DrawString(pos.x + 85 , pos.y, ", ");
        DrawString(pos.x + 95 , pos.y, key2, olc::GREEN);
        DrawString(pos.x + 100, pos.y, " to ");
        DrawString(pos.x + 130, pos.y, action);
    }

    void Sim::drawElement(std::shared_ptr<CircuitElement> element)
    {
        element->drawYourself(this);
    }

    void Sim::drawCircuit()
    {
        for(unsigned int i = 0; i < circuit.getElements().size(); i++)
        {
            drawElement(circuit.getElements()[i]); 
        }
    }

    void Sim::resetTempCoord()
    {
        tempPos.x = GetScreenSize().x / 2;
        tempPos.y = GetScreenSize().y / 2;
    }

    void Sim::WorldToScreen(const olc::vf2d& v, int& screenX, int& screenY)
    {
        screenX = (int)((v.x - worldOffset.x) * scale);
        screenY = (int)((v.y - worldOffset.y) * scale);
    }

    void Sim::ScreenToWorld(int screenX, int screenY, olc::vf2d& v)
    {
        v.x = (float)(screenX) / scale + worldOffset.x;
        v.y = (float)(screenY) / scale + worldOffset.y;
    }

    void Sim::addElem()
    {
        if(addElement)
        {
            editMenuActive = false;
            mainMenuActive = false;
            addMenuActive = false;
            removeMenuActive = false;
            modifyMenuActive = false;

            olc::vf2d worldPos;
            ScreenToWorld((int)tempPos.x, (int)tempPos.y, worldPos);
            
            int nx, ny;
            worldPos.x = floorf(worldPos.x) + 1;
            worldPos.y = floorf(worldPos.y) + 1;
            WorldToScreen(worldPos, nx, ny);
            FillCircle(nx, ny, 1);

            if(GetKey(olc::ENTER).bPressed) // add element to circuit
            {
                std::shared_ptr<CircuitElement> element;
                switch(tempType)
                {
                    case ElementType::ELEM_CABLE:
                        element = std::shared_ptr<Cable> (new Cable(worldPos.x, worldPos.y));
                        break;
                    case ElementType::ELEM_NODE:
                        element = std::shared_ptr<CableNode> (new CableNode(worldPos.x, worldPos.y));
                        break;
                    case ElementType::ELEM_RESISTOR:
                        element = std::shared_ptr<Resistor> (new Resistor(worldPos.x, worldPos.y));
                        break;
                    case ElementType::ELEM_TRANSISTOR:
                        element = std::shared_ptr<Transistor> (new Transistor(worldPos.x, worldPos.y));
                        break;
                    case ElementType::ELEM_SOURCE:
                        element = std::shared_ptr<Source> (new Source(worldPos.x, worldPos.y));
                        break;
                    case ElementType::ELEM_BATTERY:
                        element = std::shared_ptr<Battery> (new Battery(worldPos.x, worldPos.y));
                        break;
                    default:
                        throw OperationFailed("Failed to create new element!");
                }
                circuit.addElementToCircuit(element);
            }
            
            if(GetKey(olc::W).bHeld)
            {
                tempPos.y -= gridInc / 2;
            }
            if(GetKey(olc::S).bHeld)
            {
                tempPos.y += gridInc / 2; 
            }
            if(GetKey(olc::A).bHeld)
            {
                tempPos.x -= gridInc / 2; 
            }
            if(GetKey(olc::D).bHeld)
            {
                tempPos.x += gridInc / 2; 
            }

            olc::vf2d mousePosBZoom;
            olc::vf2d mousePosAZoom;
		        ScreenToWorld((int)tempPos.x, (int)tempPos.y, mousePosBZoom);
            
            if(GetKey(olc::MINUS).bHeld)
            {
                scale *= 0.999f;
            }

            if(GetKey(olc::EQUALS).bHeld)
            {
                scale *= 1.001f;
            }
           
            ScreenToWorld((int)tempPos.x, (int)tempPos.y, mousePosAZoom);
            worldOffset += (mousePosBZoom - mousePosAZoom);
        }
    }

    void Sim::drawMainMenu()
    {
        if(mainMenuActive)
        {
            FillRect(50 + menuOffset.x, 50 + menuOffset.y, 500, 200, olc::BLACK);
            DrawRect(50 + menuOffset.x, 50 + menuOffset.y, 500, 200, olc::WHITE);
            DrawString(70 + menuOffset.x, 70 + menuOffset.y, "CCircuit Designer - Commands", olc::WHITE, 2);
            DrawString(70 + menuOffset.x, 90 + menuOffset.y, "Press");
            DrawString(115 + menuOffset.x, 90 + menuOffset.y, "ESC", olc::GREEN);
            DrawString(145 + menuOffset.x, 90 + menuOffset.y, "to open/close this window");
            pressEntry({70 + menuOffset.x, 110 + menuOffset.y}, "Q", "open edit menu");
            pressEntry({70 + menuOffset.x, 130 + menuOffset.y}, "R", "start/stop simulation");
            pressEntry({70 + menuOffset.x, 150 + menuOffset.y}, "SHIFT", "W", "move window up");
            pressEntry({70 + menuOffset.x, 160 + menuOffset.y}, "SHIFT", "S", "move window down");
            pressEntry({70 + menuOffset.x, 170 + menuOffset.y}, "SHIFT", "A", "move window left");
            pressEntry({70 + menuOffset.x, 180 + menuOffset.y}, "SHIFT", "D", "move window right");
        }
    }

    void Sim::drawEditMenu()
    {
        if(editMenuActive)
        {
            FillRect(50 + menuOffset.x, 50 + menuOffset.y, 500, 200, olc::BLACK);
            DrawRect(50 + menuOffset.x, 50 + menuOffset.y, 500, 200, olc::WHITE);
            DrawString(70 + menuOffset.x, 70 + menuOffset.y, "Edit circuit", olc::WHITE, 2);
            pressEntry({70 + menuOffset.x, 90 + menuOffset.y}, "Q", "open/close this window");
            pressEntry({70 + menuOffset.x, 110 + menuOffset.y}, "1","add elements to circuit");
            pressEntry({70 + menuOffset.x, 130 + menuOffset.y}, "2","remove elements from circuit");
            pressEntry({70 + menuOffset.x, 150 + menuOffset.y}, "3","edit elements in circuit");
            if(GetKey(olc::K1).bPressed)
            {
                addMenuActive = true;
                editMenuActive = false;
            }
            if(GetKey(olc::K2).bPressed)
            {
                removeMenuActive = true;
                editMenuActive = false;
            }
            if(GetKey(olc::K3).bPressed)
            {
                modifyMenuActive = true;
                editMenuActive = false;
            }
        }
    }

    void Sim::drawAddMenu()
    {
        if(addMenuActive)
        {
            FillRect(50 + menuOffset.x, 50 + menuOffset.y, 500, 200, olc::BLACK);
            DrawRect(50 + menuOffset.x, 50 + menuOffset.y, 500, 200, olc::WHITE);
            DrawString(70 + menuOffset.x, 70 + menuOffset.y, "Edit circuit", olc::WHITE, 2);
            pressEntry({70 + menuOffset.x, 90 + menuOffset.y}, "Q", "go back to edit menu");
            pressEntry({70 + menuOffset.x, 110 + menuOffset.y}, "1", "add cable");
            pressEntry({70 + menuOffset.x, 130 + menuOffset.y}, "2", "add cable node");
            pressEntry({70 + menuOffset.x, 150 + menuOffset.y}, "3", "add resistor");
            pressEntry({70 + menuOffset.x, 170 + menuOffset.y}, "4", "add transistor");
            pressEntry({70 + menuOffset.x, 190 + menuOffset.y}, "5", "add source");
            pressEntry({70 + menuOffset.x, 210 + menuOffset.y}, "6", "add battery");

            if(GetKey(olc::K1).bPressed)
            {
                addElement = true;
                tempType = ElementType::ELEM_CABLE;
                resetTempCoord();
            }
            else if(GetKey(olc::K2).bPressed)
            {
                addElement = true;
                tempType = ElementType::ELEM_NODE;
                resetTempCoord();
            }
            else if(GetKey(olc::K3).bPressed)
            {
                addElement = true;
                tempType = ElementType::ELEM_RESISTOR;
                resetTempCoord();
            }
            else if(GetKey(olc::K4).bPressed)
            {
                addElement = true;
                tempType = ElementType::ELEM_TRANSISTOR;
                resetTempCoord();
            }
            else if(GetKey(olc::K5).bPressed)
            {
                addElement = true;
                tempType = ElementType::ELEM_SOURCE;
                resetTempCoord();
            }
            else if(GetKey(olc::K6).bPressed)
            {
                addElement = true;
                tempType = ElementType::ELEM_BATTERY;
                resetTempCoord();
            }
        }
    }

    void Sim::drawDeleteMenu()
    {
    }

    void Sim::drawModifyMenu()
    {
    }

    void Sim::drawGrid()
    {
        int32_t sx, sy;

        olc::vf2d worldTopLeft, worldBottomRight;
        ScreenToWorld(0, 0, worldTopLeft);
        ScreenToWorld(ScreenWidth(), ScreenHeight(), worldBottomRight);

        worldTopLeft.x = floor(worldTopLeft.x);
        worldTopLeft.y = floor(worldTopLeft.y);
        worldBottomRight.x = ceil(worldBottomRight.x);
        worldBottomRight.y = ceil(worldBottomRight.y);

        for (float x = worldTopLeft.x; x < worldBottomRight.x; x += gridInc)
        {
            for(float y = worldTopLeft.y; y < worldBottomRight.y; y += gridInc)
            {
                WorldToScreen({x, y}, sx, sy);
                Draw(sx, sy, olc::BLUE);
            }
        }
    }

    void Sim::checkGlobalKeyPress()
    {
        if(GetKey(olc::ESCAPE).bPressed)
        {
            if(!addElement)
                mainMenuActive = !mainMenuActive;
            
            editMenuActive = false;
            addMenuActive = false;
            addElement = false;
            resetOffset(); 
        }

        if(GetKey(olc::Q).bPressed)
        {
            editMenuActive = !editMenuActive;
            mainMenuActive = false;
            addMenuActive = false;
            resetOffset();
        }
        
        if(GetKey(olc::SHIFT).bHeld && GetKey(olc::W).bHeld) // move window up
            menuOffset.y-=(int)(std::sqrt(scale));
        if(GetKey(olc::SHIFT).bHeld && GetKey(olc::S).bHeld) // move window down
            menuOffset.y+=(int)(std::sqrt(scale));
        if(GetKey(olc::SHIFT).bHeld && GetKey(olc::A).bHeld) // move window right
            menuOffset.x-=(int)(std::sqrt(scale));
        if(GetKey(olc::SHIFT).bHeld && GetKey(olc::D).bHeld) // move window left
            menuOffset.x+=(int)(std::sqrt(scale));

    }

    void Sim::mouseControls()
    {
        mousePos = {(float)GetMouseX(), (float)GetMouseY() };

        if(GetMouse(2).bPressed)
        {
            startPan = mousePos;
        }

        if(GetMouse(2).bHeld)
        {
            worldOffset -= (mousePos - startPan) / scale; 
            startPan = mousePos;
        }

        olc::vf2d mousePosBZoom;
        ScreenToWorld((int)mousePos.x, (int)mousePos.y, mousePosBZoom);

        if(GetMouseWheel() > 0)
            scale *= 1.01f;
        if(GetMouseWheel() < 0)
            scale *= 0.99f;

        olc::vf2d mousePosAZoom;
        ScreenToWorld((int)mousePos.x, (int)mousePos.y, mousePosAZoom);
        worldOffset += (mousePosBZoom - mousePosAZoom);

    }

    Sim::Sim()
    {
        sAppName = "Circuit Simulator";
    }

    // cppcheck-suppress unusedFunction
    [[maybe_unused]] bool Sim::OnUserCreate()
    {
        worldOffset.x = (float)(-GetScreenSize().x / 2) / scale;
        worldOffset.y = (float)(-GetScreenSize().y / 2) / scale;

        mainMenuActive = true;
        resetTempCoord();

        return true;
    }

    // cppcheck-suppress unusedFunction
    [[maybe_unused]] bool Sim::OnUserUpdate([[maybe_unused]] float fElapsedTime)
    {
        Clear(olc::BLACK);

        mouseControls();
        checkGlobalKeyPress();
        
        CircuitElement::setWorldOffset(worldOffset);
        CircuitElement::setWorldScale(scale);

        drawGrid();
        drawCircuit(); 

        try{
            addElem();
        } // couldn't add a new element for whatever reason
        catch (OperationFailed const &)
        {
            addElement = false;
        }

        drawMainMenu();
        drawEditMenu();
        drawAddMenu();
        drawModifyMenu();
        drawDeleteMenu();
        
        return true;
    }

