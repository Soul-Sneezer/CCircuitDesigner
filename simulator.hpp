#ifndef CC_SIMULATOR
#define CC_SIMULATOR

#include <olcPixelGameEngine.h>
#include "circuit_elements.hpp"
#include "circuit.hpp"
#include "circuit_factory.hpp"
#include "debug.hpp"
#include "menu.hpp"

class Sim : public olc::PixelGameEngine
{
    private:
        std::vector<std::pair<std::shared_ptr<Menu>, bool>> menus;
        bool exit = false;

        const int radius = 4;

        Menu mainMenu;
        bool mainMenuActive = false;

        Menu editMenu;
        bool editMenuActive = false;

        Menu removeMenu;
        bool removeMenuActive = false;

        Menu addMenu;
        bool addMenuActive = false;

        Menu modifyMenu;
        bool modifyMenuActive = false;
        bool addElement = false;

        olc::vi2d menuOffset = {0, 0};
        olc::vf2d worldOffset = {0.0f, 0.0f};

        float scale = 8.0f;
        float gridInc = 1.0f;

        olc::vf2d startPan;

        ElementType tempType = ElementType::ELEM_UNASSIGNED;
        olc::vf2d tempPos;

        std::shared_ptr<Circuit> circuit;
        std::shared_ptr<Factory> factory;

        void resetOffset();
        void resetTempCoord();

        void createMenus();
        void createMainMenu();
        void createEditMenu();
        void createModifyMenu();
        void createRemoveMenu();
        void createAddMenu();

        void drawMenus();

        void WorldToScreen(const olc::vf2d& v, int& screenX, int& screenY);
        void ScreenToWorld(int screenX, int screenY, olc::vf2d& v);

        std::pair<olc::vf2d, olc::vf2d> addLine();
        void addElem();

        void selectElement();
        void drawDeleteMenu();
        void drawModifyMenu();

        void drawGrid();

        void addMenuToMenus(std::shared_ptr<Menu> menu);
        void mouseControls();
        void keyboardControls();
    public:
        Sim();

        // cppcheck-suppress unusedFunction
        [[maybe_unused]] bool OnUserCreate() override;
        // cppcheck-suppress unusedFunction
        [[maybe_unused]] bool OnUserUpdate([[maybe_unused]] float fElapsedTime) override;
};

#endif
