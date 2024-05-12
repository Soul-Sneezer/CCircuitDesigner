#ifndef CC_SIMULATOR
#define CC_SIMULATOR

#include <olcPixelGameEngine.h>
#include "circuit_elements.hpp"
#include "circuit.hpp"

class OperationFailed : public std::runtime_error
{
    public:
        explicit OperationFailed(const char* message) throw();
};

class Sim : public olc::PixelGameEngine
{
    private:
        bool mainMenuActive = false;
        bool editMenuActive = false;
        bool removeMenuActive = false;
        bool addMenuActive = false;
        bool modifyMenuActive = false;
        bool addElement = false;

        olc::vi2d menuOffset = {0, 0};
        olc::vf2d worldOffset = {0.0f, 0.0f};

        float scale = 8.0f;
        float gridInc = 1.0f;

        olc::vf2d mousePos;
        olc::vf2d startPan;

        ElementType tempType = ElementType::ELEM_UNASSIGNED;
        olc::vf2d tempPos;

        Circuit circuit;

        void resetOffset();

        void pressEntry(const olc::vi2d pos, const std::string& key, const std::string& action);
        void pressEntry(const olc::vi2d pos, const std::string& key1, const std::string& key2, const std::string& action);

        void drawElement(CircuitElement* element);
        void drawCircuit();

        void resetTempCoord();

        void WorldToScreen(const olc::vf2d& v, int& screenX, int& screenY);
        void ScreenToWorld(int screenX, int screenY, olc::vf2d& v);

        void addElem();
        void drawMainMenu();
        void drawEditMenu();
        void drawAddMenu();
        void drawDeleteMenu();
        void drawModifyMenu();

        void drawGrid();

        void checkKeyPress();
        void mouseControls();

    public:
        Sim();

        // cppcheck-suppress unusedFunction
        [[maybe_unused]] bool OnUserCreate() override;
        // cppcheck-suppress unusedFunction
        [[maybe_unused]] bool OnUserUpdate([[maybe_unused]] float fElapsedTime) override;
};

#endif
