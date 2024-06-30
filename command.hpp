#ifndef CC_COMMAND
#define CC_COMMAND

#include <unordered_map>
#include "menu.hpp"
#include "simulator.hpp"
#include "circuit_elements.hpp"

class Command
{
public:
    explicit Command();
    virtual ~Command() {}
    virtual void execute(Sim* sim) = 0;
};

class QuitCommand : public Command
{   
public:
    explicit QuitCommand();

    void execute(Sim* sim) override;
};

class MoveMenuCommand : public Command
{
private:
    int x, y;
public:
    explicit MoveMenuCommand(int x, int y);
    
    void execute(Sim* sim) override;
};

class ZoomCommand : public Command
{
private:
    int scale;
public:
    explicit ZoomCommand(float value);

    void execute(Sim* sim) override;
};

class SwitchMenuCommand : public Command
{
private:
    int menuIndex1, menuIndex2;
public:
    explicit SwitchMenuCommand(int menuIndex1, int menuIndex2);

    void execute(Sim* sim) override;
};

class SelectElemCommand : public Command
{
private:
    ElementType type;
public:
    explicit SelectElemCommand(ElementType type);

    void execute(Sim* sim) override;
};

class RunSimulationCommand : public Command
{
public:
    explicit RunSimulationCommand();

    void execute(Sim* sim) override;
};

class InputHandler
{
private:
    std::unordered_map<olc::Key, std::shared_ptr<Command>> keyMapping; // for remapping the keys;
    std::shared_ptr<Command> buttonW = nullptr;
    std::shared_ptr<Command> buttonA = nullptr;
    std::shared_ptr<Command> buttonS = nullptr;
    std::shared_ptr<Command> buttonD = nullptr;
    std::shared_ptr<Command> buttonE = nullptr;
    std::shared_ptr<Command> buttonR = nullptr;
    std::shared_ptr<Command> buttonX = nullptr;
    std::shared_ptr<Command> button1 = nullptr;
    std::shared_ptr<Command> button2 = nullptr;
    std::shared_ptr<Command> button3 = nullptr;
    std::shared_ptr<Command> button4 = nullptr;
    std::shared_ptr<Command> button5 = nullptr;
    std::shared_ptr<Command> buttonEqual = nullptr;
    std::shared_ptr<Command> buttonMinus = nullptr;
public:
    InputHandler();
    ~InputHandler();

    std::shared_ptr<Command> handleInput(Sim* sim, olc::PixelGameEngine* pge);
};

#endif
