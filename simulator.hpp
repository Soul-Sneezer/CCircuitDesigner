#ifndef CC_SIMULATOR
#define CC_SIMULATOR

#include <olcPixelGameEngine.h>
#include "circuit_elements.hpp"
#include "circuit.hpp"
#include "circuit_factory.hpp"
#include "debug.hpp"
#include "menu.hpp"

class Sim
{
    private:
        olc::vf2d worldOffset = {0.0f, 0.0f};
        const float gridInc = 1.0f;

        std::shared_ptr<Circuit> circuit;
        std::shared_ptr<Factory> factory;

        std::vector<std::pair<std::shared_ptr<Menu>, bool>> menus;
        bool addElement = false;

        const float radius = 4.0f;
        float scale = 8.0f;
        
        bool exit = false;
        olc::vf2d startPan;

        ElementType tempType = ElementType::ELEM_UNASSIGNED;
        olc::vf2d tempPos;
        
        void createMenus();
        void createMainMenu();
        void createEditMenu();
        void createModifyMenu();
        void createRemoveMenu();
        void createAddMenu();

        void addMenuToMenus(std::shared_ptr<Menu> menu);

        void WorldToScreen(const olc::vf2d& v, int& screenX, int& screenY);
        void ScreenToWorld(int screenX, int screenY, olc::vf2d& v);
public:
        Sim();

        std::shared_ptr<Circuit> getCircuit();

        float getScale();
        void setScale(float newValue);
        std::vector<std::pair<std::shared_ptr<Menu>, bool>> getMenus();
        ElementType getTempType();
        void setTempType(ElementType type);
        bool getExitStatus();
        olc::vf2d getTempPos();
        olc::vi2d getWorldOffset();

        void initializeWorldOffset(olc::PixelGameEngine* pge);
        void resetTempCoord(olc::PixelGameEngine* pge);

        void setAddElementFalse();
        void drawMenus(olc::PixelGameEngine* pge);
        std::pair<olc::vf2d, olc::vf2d> addLine(olc::PixelGameEngine* pge);
        void addElem(olc::PixelGameEngine* pge);
        void drawGrid(olc::PixelGameEngine* pge);
        void drawCircuit(olc::PixelGameEngine* pge);
};

#endif
