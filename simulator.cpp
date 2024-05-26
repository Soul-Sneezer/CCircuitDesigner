#include "simulator.hpp"

    OperationFailed::OperationFailed(const char* message) throw() : std::runtime_error(message)
    {

    }

    void Sim::resetOffset()
    {
        menuOffset = {0,0};
    }

    void Sim::pressEntry(const olc::vi2d pos, const std::string& key, const std::string& action, const uint32_t fontSize = 1)
    {
        DrawString(pos.x + menuOffset.x                , pos.y + menuOffset.y , "Press ", olc::WHITE, fontSize);
        DrawString(pos.x + menuOffset.x + 48 * fontSize, pos.y + menuOffset.y , key,      olc::GREEN, fontSize);
        DrawString(pos.x + menuOffset.x + (48 + 8 * key.size()) * fontSize, pos.y + menuOffset.y , " to ",   olc::WHITE, fontSize);
        DrawString(pos.x + menuOffset.x + (80 + 8 * key.size()) * fontSize, pos.y + menuOffset.y , action,   olc::WHITE, fontSize);
    }
    void Sim::pressEntry(const olc::vi2d pos, const std::string& key1, const std::string& key2, const std::string& action, const uint32_t fontSize = 1)
    {
        DrawString(pos.x + menuOffset.x                 , pos.y + menuOffset.y , "Press ", olc::WHITE, fontSize);
        DrawString(pos.x + menuOffset.x + 48  * fontSize, pos.y + menuOffset.y , key1,     olc::GREEN, fontSize);
        DrawString(pos.x + menuOffset.x + (48 + 8 * key1.size())  * fontSize, pos.y + menuOffset.y , ",",      olc::WHITE, fontSize);
        DrawString(pos.x + menuOffset.x + (56 + 8 * key1.size())  * fontSize, pos.y + menuOffset.y , key2,      olc::GREEN, fontSize);
        DrawString(pos.x + menuOffset.x + (56 + 8 * key1.size() + 8 * key2.size())  * fontSize, pos.y + menuOffset.y , " to ",   olc::WHITE, fontSize);
        DrawString(pos.x + menuOffset.x + (88 + 8 * key1.size() + 8 * key2.size()) * fontSize, pos.y + menuOffset.y , action,   olc::WHITE, fontSize);

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

    void Sim::addLine()
    {
        olc::vf2d worldPos;
        olc::vf2d worldPos1;
        olc::vf2d worldPos2;
        olc::vf2d mousePos = {(float)GetMouseX(), (float)GetMouseY() };
        ScreenToWorld((int)mousePos.x, (int)mousePos.y, worldPos);
       
        worldPos2.x = floorf(worldPos.x + gridInc);
        worldPos2.y = floorf(worldPos.y + gridInc);
        worldPos1.x = floorf(worldPos.x);
        worldPos1.y = floorf(worldPos.y);

        float distX1 = worldPos.x - worldPos1.x;
        float distX2 = worldPos2.x - worldPos.x;
        float distY1 = worldPos.y - worldPos1.y;
        float distY2 = worldPos2.y - worldPos.y;
        
        int coordX1, coordX2, coordY1, coordY2;

        WorldToScreen(worldPos1, coordX1, coordY1);
        WorldToScreen(worldPos2, coordX2, coordY2);

        float min = distX1;
        
        if(distX2 < min)
            min = distX2;
        if(distY1 < min)
            min = distY1;
        if(distY2 < min)
            min = distY2;
       
        if(min == distX1)
        {
            DrawLine(coordX1, coordY1, coordX1, coordY2, olc::WHITE);
        }
        else if(min == distX2)
        {
            DrawLine(coordX2, coordY1, coordX2, coordY2, olc::WHITE);
        }
        else if(min == distY1)
        {
            DrawLine(coordX1, coordY1, coordX2, coordY1, olc::WHITE);
        }
        else
        {
            DrawLine(coordX1, coordY2, coordX2, coordY2, olc::WHITE);
        }

        //DrawLine(coordX1, coordY1, coordX2, coordY2, olc::WHITE);
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
            olc::vf2d mousePos = {(float)GetMouseX(), (float)GetMouseY() };
            ScreenToWorld((int)mousePos.x, (int)mousePos.y, worldPos);

            worldPos.x = floorf(worldPos.x + gridInc / 2);
            worldPos.y = floorf(worldPos.y + gridInc / 2);
            addLine();
            int nx, ny;
            WorldToScreen(worldPos, nx, ny);
            FillCircle(nx , ny, radius / std::min(2, (int)scale));

            if(GetKey(olc::ENTER).bPressed || GetMouse(0).bPressed) // add element to circuit
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
            
            olc::vf2d mousePosBZoom;
            olc::vf2d mousePosAZoom;
		        ScreenToWorld((int)tempPos.x, (int)tempPos.y, mousePosBZoom);
            
            if(GetKey(olc::MINUS).bHeld)
            {
                if(scale > 10.0f)
                    scale *= 0.99f;
                else
                    scale = 10.0f;
            }

            if(GetKey(olc::EQUALS).bHeld)
            {
                if(scale < 100.0f)
                    scale *= 1.01f;
                else
                    scale = 100.0f;
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
            pressEntry({70, 90}, "ESC", "to open/close this window");
            pressEntry({70, 110}, "Q", "open edit menu");
            pressEntry({70, 130}, "R", "start/stop simulation");
            pressEntry({70, 150}, "SHIFT", "W", "move window up");
            pressEntry({70, 160}, "SHIFT", "S", "move window down");
            pressEntry({70, 170}, "SHIFT", "A", "move window left");
            pressEntry({70, 180}, "SHIFT", "D", "move window right");
        }
    }

    void Sim::drawEditMenu()
    {
        if(editMenuActive)
        {
            FillRect(50 + menuOffset.x, 50 + menuOffset.y, 500, 200, olc::BLACK);
            DrawRect(50 + menuOffset.x, 50 + menuOffset.y, 500, 200, olc::WHITE);
            DrawString(70 + menuOffset.x, 70 + menuOffset.y, "Edit circuit", olc::WHITE, 2);
            pressEntry({70,  90}, "Q", "open/close this window");
            pressEntry({70, 110}, "1","add elements to circuit");
            pressEntry({70, 130}, "2","remove elements from circuit");
            pressEntry({70, 150}, "3","edit elements in circuit");
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
            pressEntry({70,  90}, "Q", "go back to edit menu");
            pressEntry({70, 110}, "1", "add cable");
            pressEntry({70, 130}, "2", "add cable node");
            pressEntry({70, 150}, "3", "add resistor");
            pressEntry({70, 170}, "4", "add transistor");
            pressEntry({70, 190}, "5", "add source");
            pressEntry({70, 210}, "6", "add battery");
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
            WorldToScreen({x, 0}, sx, sy);
            DrawLine(sx, 0, sx, ScreenHeight(), olc::BLUE);
            for(float y = worldTopLeft.y; y < worldBottomRight.y; y += gridInc)
            {
                WorldToScreen({x, y}, sx, sy);
                FillCircle(sx, sy, radius / std::min(2, (int)scale), olc::BLUE);
            }
        }

        for(float y = worldTopLeft.y; y < worldBottomRight.y; y += gridInc)
        {
            WorldToScreen({0, y}, sx, sy);
            DrawLine(0, sy, ScreenWidth(), sy, olc::BLUE);
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
        olc::vf2d mousePos = {(float)GetMouseX(), (float)GetMouseY() };

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
        {
            if(scale < 100.0f)
                scale *= 1.1f;
            else
                scale = 100.0f;
        }
        if(GetMouseWheel() < 0)
        {
            if(scale > 10.0f)
                scale *= 0.9f;
            else
                scale = 10.0f;
        }
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

        drawAddMenu();

        drawMainMenu();
        drawEditMenu();
        drawModifyMenu();
        drawDeleteMenu();
        
        try{
            addElem();
        } // couldn't add a new element for whatever reason
        catch (OperationFailed const &)
        {
            addElement = false; 
        }

        return true;
    }

