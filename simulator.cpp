#include "simulator.hpp"
    void Sim::setAddElementFalse()
    {
        this->addElement = false;
    }

    olc::vi2d Sim::getWorldOffset()
    {
        return this->worldOffset;
    }

    void Sim::initializeWorldOffset(olc::PixelGameEngine* pge)
    {
        worldOffset.x = (float)(-pge->GetScreenSize().x / 2) / scale;
        worldOffset.y = (float)(-pge->GetScreenSize().y / 2) / scale;
    }

    olc::vf2d Sim::getTempPos()
    {
        return this->tempPos;
    }

    bool Sim::getExitStatus()
    {
        return this->exit;
    }

    std::shared_ptr<Circuit> Sim::getCircuit()
    {
        return this->circuit;
    }

    float Sim::getScale()
    {
        return this->scale;
    }

    void Sim::setScale(const float newValue)
    {
        this->scale = newValue;
    }

    std::vector<std::pair<std::shared_ptr<Menu>, bool>> Sim::getMenus()
    {
        return this->menus;
    }

// cppcheck-suppress unusedFunction
    ElementType Sim::getTempType()
    {
        return this->tempType;
    }

    void Sim::setTempType(const ElementType type)
    {
        this->tempType = type;
    }

    void Sim::addMenuToMenus(std::shared_ptr<Menu> menu)
    {
        this->menus.push_back(std::make_pair(menu, false));
    }

    void Sim::createMenus()
    {
        createMainMenu();
        createEditMenu();
        createModifyMenu();
        createAddMenu();
        createRemoveMenu();

        menus[0].second = true;
    }

    std::shared_ptr<MenuContent> toMenuContent(std::shared_ptr<Text> content)
    {
        return dynamic_pointer_cast<MenuContent>(content);
    }

    std::shared_ptr<MenuContent> toMenuContent(std::shared_ptr<PressEntry> content)
    {
        return dynamic_pointer_cast<MenuContent>(content);
    }

    void Sim::createMainMenu()
    {
        std::shared_ptr<Menu> mainMenu = std::make_shared<Menu>(50, 50, 500, 200);
       
        mainMenu->addContentToMenu(toMenuContent(std::make_shared<Text>(10, 10, "CCircuit Designer - Commands", 1)));
        mainMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(20, 30, "ESC", "open/close this window", 1)));
        mainMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(20, 50, "X", "close program", 1)));
        mainMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(20, 70, "Q", "open edit menu", 1)));
        mainMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(20, 90, "R", "start/stop simulation", 1)));
        mainMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(20, 110, "SHIFT", "W", "move window up", 1)));
        mainMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(20, 130, "SHIFT", "S", "move window down", 1)));
        mainMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(20, 150, "SHIFT", "A", "move window left", 1)));
        mainMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(20, 170, "SHIFT", "D", "move window right", 1)));

        addMenuToMenus(mainMenu);
    }

    void Sim::createEditMenu()
    {
        std::shared_ptr<Menu> editMenu = std::make_shared<Menu>(50, 50, 500, 200);
        editMenu->addContentToMenu(toMenuContent(std::make_shared<Text>(10, 10, "Edit circuit", 1)));
        editMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(20,  30, "Q", "open/close this window", 1)));
        editMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(20, 50, "1","add elements to circuit", 1)));
        editMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(20, 70, "2","remove elements from circuit", 1)));
        editMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(20, 90, "3","modify elements in circuit", 1)));
    
        addMenuToMenus(editMenu);
    }

    void Sim::createModifyMenu()
    {
        std::shared_ptr<Menu> modifyMenu = std::make_shared<Menu>(50, 50, 500, 80);

        modifyMenu->addContentToMenu(toMenuContent(std::make_shared<Text>(10, 10, "Modify circuit", 1)));
        modifyMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(20,  30, "Q", "go back to edit menu", 1)));
    
        addMenuToMenus(modifyMenu);
    }

    void Sim::createAddMenu()
    {
        std::shared_ptr<Menu> addMenu = std::make_shared<Menu>(50, 50, 500, 200);

        addMenu->addContentToMenu(toMenuContent(std::make_shared<Text>(10, 10, "Add element to circuit", 1)));
        addMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(20,  30, "Q", "go back to edit menu", 1)));
        addMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(20, 50, "1", "add cable", 1)));
        addMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(20, 70, "2", "add resistor", 1)));
        addMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(20, 90, "3", "add transistor", 1)));
        addMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(20, 110, "4", "add source", 1)));
        addMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(20, 130, "5", "add battery", 1)));
    
        addMenuToMenus(addMenu);
    }

    void Sim::createRemoveMenu()
    {
        std::shared_ptr<Menu> removeMenu = std::make_shared<Menu>(50, 50, 500, 80);
        
        removeMenu->addContentToMenu(toMenuContent(std::make_shared<Text>(10, 10, "Remove element from circuit", 1)));
        removeMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(20,  30, "Q", "go back to edit menu", 1)));
    
        addMenuToMenus(removeMenu);
    }

    void Sim::drawMenus(olc::PixelGameEngine* pge)
    {
        for(auto menu : menus)
        {
            if(menu.second)
                (menu.first)->drawYourself(pge);
        }
    }

    // cppcheck-suppress unusedFunction
    void Sim::resetTempCoord(olc::PixelGameEngine* pge)
    {
        tempPos.x = pge->GetScreenSize().x / 2;
        tempPos.y = pge->GetScreenSize().y / 2;
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

    std::pair<olc::vf2d, olc::vf2d> Sim::addLine(olc::PixelGameEngine* pge)
    {
        olc::vf2d worldPos;
        olc::vf2d worldPos1;
        olc::vf2d worldPos2;
        olc::vf2d mousePos = {(float)pge->GetMouseX(), (float)pge->GetMouseY() };
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
            pge->DrawLine(coordX1, coordY1, coordX1, coordY2, olc::WHITE);
            worldPos2.x = worldPos1.x;
        }
        else if(min == distX2)
        {
            pge->DrawLine(coordX2, coordY1, coordX2, coordY2, olc::WHITE);
            worldPos1.x = worldPos2.x;
        }
        else if(min == distY1)
        {
            pge->DrawLine(coordX1, coordY1, coordX2, coordY1, olc::WHITE);
            worldPos2.y = worldPos1.y; 
        }
        else
        {
            pge->DrawLine(coordX1, coordY2, coordX2, coordY2, olc::WHITE);
            worldPos1.y = worldPos2.y;
        }
      
        return std::make_pair(worldPos1, worldPos2);

    }

    void Sim::addElem(olc::PixelGameEngine* pge)
    {
        if(addElement)
        {
            olc::vf2d worldPos;
            std::pair<olc::vf2d, olc::vf2d> linePos;
            olc::vf2d mousePos = {(float)pge->GetMouseX(), (float)pge->GetMouseY() };
            ScreenToWorld((int)mousePos.x, (int)mousePos.y, worldPos);

            worldPos.x = floorf(worldPos.x + gridInc / 2);
            worldPos.y = floorf(worldPos.y + gridInc / 2);
           
            int nx, ny;
            WorldToScreen(worldPos, nx, ny);
            pge->FillCircle(nx , ny, radius / std::min(2, (int)scale));
            linePos = addLine(pge);

            if(pge->GetKey(olc::ENTER).bPressed || pge->GetMouse(0).bPressed) // add element to circuit
            {
                std::shared_ptr<CircuitElement> elem = factory->makeElement(tempType, linePos);
                circuit->addElem(elem);
            }     
        }
    }

    void Sim::drawGrid(olc::PixelGameEngine* pge)
    {
        int32_t sx, sy;

        olc::vf2d worldTopLeft, worldBottomRight;
        ScreenToWorld(0, 0, worldTopLeft);
        ScreenToWorld(pge->ScreenWidth(), pge->ScreenHeight(), worldBottomRight);

        worldTopLeft.x = floor(worldTopLeft.x);
        worldTopLeft.y = floor(worldTopLeft.y);
        worldBottomRight.x = ceil(worldBottomRight.x);
        worldBottomRight.y = ceil(worldBottomRight.y);

        for (float x = worldTopLeft.x; x < worldBottomRight.x; x += gridInc)
        {
            WorldToScreen({x, 0}, sx, sy);
            pge->DrawLine(sx, 0, sx, pge->ScreenHeight(), olc::BLUE);
            for(float y = worldTopLeft.y; y < worldBottomRight.y; y += gridInc)
            {
                WorldToScreen({x, y}, sx, sy);
                pge->FillCircle(sx, sy, radius / std::min(2, (int)scale), olc::BLUE);
            }
        }

        for(float y = worldTopLeft.y; y < worldBottomRight.y; y += gridInc)
        {
            WorldToScreen({0, y}, sx, sy);
            pge->DrawLine(0, sy, pge->ScreenWidth(), sy, olc::BLUE);
        }
    }

    void Sim::drawCircuit(olc::PixelGameEngine* pge)
    {
        circuit->drawCircuit(pge);     
    }
