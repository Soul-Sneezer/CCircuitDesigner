#ifndef CC_COMMAND
#define CC_COMMAND

#include <unordered_map>
#include "menu.hpp"
#include "simulator.hpp"
#include "circuit_elements.hpp"

class Command
{
protected:
    std::shared_ptr<Sim> sim;
public:
    Command(std::shared_ptr<Sim> sim);
    virtual ~Command() {}
    virtual void execute() = 0;
};

class QuitCommand : public Command
{   
public:
    QuitCommand(std::shared_ptr<Sim> sim);

    void execute() override;
};

class MoveMenuCommand : public Command
{
private:
    int x, y;
public:
    MoveMenuCommand(std::shared_ptr<Sim> sim, int x, int y);
    
    void execute() override;
};

class ZoomCommand : public Command
{
private:
    int scaleBefore;
    int scale;
public:
    ZoomCommand(std::shared_ptr<Sim> sim, float value);

    void execute() override;
    void undo();
};

class SwitchMenuCommand : public Command
{
private:
    int menuIndex1, menuIndex2;
public:
    SwitchMenuCommand(std::shared_ptr<Sim> sim, int menuIndex1, int menuIndex2);

    void execute() override;
};

class SelectElemCommand : public Command
{
private:
    ElementType type;
public:
    SelectElemCommand(std::shared_ptr<Sim> sim, ElementType type);

    void execute() override;
};

class RunSimulationCommand : public Command
{
public:
    RunSimulationCommand(std::shared_ptr<Sim> sim);

    void execute() override;
};

class InputHandler
{
private:
    std::shared_ptr<Sim> sim;

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
    bool isPressed(olc::Key c);
public:
    InputHandler(std::shared_ptr<Sim> sim);
    ~InputHandler();

    std::shared_ptr<Command> handleInput();
};

#endif
