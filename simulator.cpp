#include "simulator.hpp"

    void Sim::resetOffset()
    {
        menuOffset = {0,0};
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

    std::pair<olc::vf2d, olc::vf2d> Sim::addLine()
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
            worldPos2.x = worldPos1.x;
        }
        else if(min == distX2)
        {
            DrawLine(coordX2, coordY1, coordX2, coordY2, olc::WHITE);
            worldPos1.x = worldPos2.x;
        }
        else if(min == distY1)
        {
            DrawLine(coordX1, coordY1, coordX2, coordY1, olc::WHITE);
            worldPos2.y = worldPos1.y; 
        }
        else
        {
            DrawLine(coordX1, coordY2, coordX2, coordY2, olc::WHITE);
            worldPos1.y = worldPos2.y;
        }
      
        return std::make_pair(worldPos1, worldPos2);

    }

    void Sim::addElem()
    {
        //if(addElement)
        //{
/*
            editMenuActive = false;
            mainMenuActive = false;
            addMenuActive = false;
            removeMenuActive = false;
            modifyMenuActive = false;
*/
            olc::vf2d worldPos;
            std::pair<olc::vf2d, olc::vf2d> linePos;
            olc::vf2d mousePos = {(float)GetMouseX(), (float)GetMouseY() };
            ScreenToWorld((int)mousePos.x, (int)mousePos.y, worldPos);

            worldPos.x = floorf(worldPos.x + gridInc / 2);
            worldPos.y = floorf(worldPos.y + gridInc / 2);
           
            int nx, ny;
            WorldToScreen(worldPos, nx, ny);
            FillCircle(nx , ny, radius / std::min(2, (int)scale));
            linePos = addLine();

            if(GetKey(olc::ENTER).bPressed || GetMouse(0).bPressed) // add element to circuit
            {
                std::shared_ptr<CircuitElement> elem = factory->makeElement(tempType, linePos);
                circuit->addElem(elem);
            }     
        //}
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
    }

    std::shared_ptr<MenuContent> toMenuContent(std::shared_ptr<PressEntry> content)
    {
        return dynamic_pointer_cast<MenuContent>(content);
    }

    void Sim::createMainMenu()
    {
        std::shared_ptr<Menu> mainMenu = std::make_shared<Menu>(50, 50, 500, 200);
        
        mainMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(70, 90, "ESC", "open/close this window", 1)));
        mainMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(70, 110, "X", "close program", 1)));
        mainMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(70, 130, "Q", "open edit menu", 1)));
        mainMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(70, 150, "R", "start/stop simulation", 1)));
        mainMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(70, 170, "SHIFT", "W", "move window up", 1)));
        mainMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(70, 180, "SHIFT", "S", "move window down", 1)));
        mainMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(70, 190, "SHIFT", "A", "move window left", 1)));
        mainMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(70, 200, "SHIFT", "D", "move window right", 1)));

        addMenuToMenus(mainMenu);
    }

    void Sim::createEditMenu()
    {
        std::shared_ptr<Menu> editMenu = std::make_shared<Menu>(50, 50, 500, 200);

        editMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(70,  90, "Q", "open/close this window", 1)));
        editMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(70, 110, "1","add elements to circuit", 1)));
        editMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(70, 130, "2","remove elements from circuit", 1)));
        editMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(70, 150, "3","edit elements in circuit", 1)));
    
        addMenuToMenus(editMenu);
    }

    void Sim::createModifyMenu()
    {
        std::shared_ptr<Menu> modifyMenu = std::make_shared<Menu>(50, 50, 500, 80);

        modifyMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(70,  90, "Q", "go back to edit menu", 1)));
    
        addMenuToMenus(modifyMenu);
    }

    void Sim::createAddMenu()
    {
        std::shared_ptr<Menu> addMenu = std::make_shared<Menu>(50, 50, 500, 200);

        addMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(70,  90, "Q", "go back to edit menu", 1)));
        addMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(70, 110, "1", "add cable", 1)));
        addMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(70, 130, "2", "add resistor", 1)));
        addMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(70, 150, "3", "add transistor", 1)));
        addMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(70, 170, "4", "add source", 1)));
        addMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(70, 190, "5", "add battery", 1)));
    
        addMenuToMenus(addMenu);
    }

    void Sim::createRemoveMenu()
    {
        std::shared_ptr<Menu> removeMenu = std::make_shared<Menu>(50, 50, 500, 80);

        removeMenu->addContentToMenu(toMenuContent(std::make_shared<PressEntry>(70,  90, "Q", "go back to edit menu", 1)));
    
        addMenuToMenus(removeMenu);
    }

    void Sim::drawMenus()
    {
        for(auto menu : menus)
        {
            if(menu.second)
                (menu.first)->drawYourself(this);
        }
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

    void Sim::keyboardControls()
    {
        /*
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
        */

        if(GetKey(olc::X).bPressed)
            exit = true;
       
        if(GetKey(olc::W).bHeld)
        {
            worldOffset.y -= 1.0f / scale;
        }

        if(GetKey(olc::A).bHeld)
        {
            worldOffset.x -= 1.0f / scale;
        }

        if(GetKey(olc::S).bHeld)
        {
            worldOffset.y += 1.0f / scale;
        }

        if(GetKey(olc::D).bHeld)
        {
            worldOffset.x += 1.0f / scale;
        }

        if(GetKey(olc::SHIFT).bHeld && GetKey(olc::W).bHeld) // move window up
            menuOffset.y-=(int)(std::sqrt(scale));
        if(GetKey(olc::SHIFT).bHeld && GetKey(olc::S).bHeld) // move window down
            menuOffset.y+=(int)(std::sqrt(scale));
        if(GetKey(olc::SHIFT).bHeld && GetKey(olc::A).bHeld) // move window right
            menuOffset.x-=(int)(std::sqrt(scale));
        if(GetKey(olc::SHIFT).bHeld && GetKey(olc::D).bHeld) // move window left
            menuOffset.x+=(int)(std::sqrt(scale));

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

    void Sim::mouseControls()
    {
        olc::vf2d mousePos = {(float)GetMouseX(), (float)GetMouseY() };

        if(GetMouse(1).bPressed)
        {
            startPan = mousePos;
        }

        if(GetMouse(1).bHeld)
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
        allocSprites();
        circuit = std::make_shared<Circuit>();
        factory = std::make_shared<Factory>();

        worldOffset.x = (float)(-GetScreenSize().x / 2) / scale;
        worldOffset.y = (float)(-GetScreenSize().y / 2) / scale;

        //mainMenuActive = true;
        resetTempCoord();
        createMenus();

        return true;
    }

    // cppcheck-suppress unusedFunction
    [[maybe_unused]] bool Sim::OnUserUpdate([[maybe_unused]] float fElapsedTime)
    {
        if(exit)
            return false;
        Clear(olc::BLACK);

        mouseControls();
        keyboardControls();
        
        CircuitElement::setWorldOffset(worldOffset);
        CircuitElement::setWorldScale(scale);

        drawGrid();
        try{
            addElem();
        } // couldn't add a new element for whatever reason
        catch (OperationFailed const &)
        {
   //         addElement = false; 
        }

        circuit->drawCircuit(this); 

        drawMenus();

        return true;
    }
