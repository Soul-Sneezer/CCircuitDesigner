#ifndef CC_COMMAND
#define CC_COMMAND

#include <unordered_map>
#include "menu.hpp"
#include "simulator.hpp"
#include "circuit_elements.hpp"

class Command
{
    public:
        virtual ~Command() {}
        virtual void execute() = 0;
        virtual void undo() = 0;
};

class QuitCommand : public Command
{
    std::shared_ptr<Sim> sim;
public:
    QuitCommand(std::shared_ptr<Sim> sim);

    void execute() override;
};

class MoveMenuCommand : public Command
{
private:
    std::shared_ptr<Menu> menu;
    int xBefore, yBefore;
    int x, y;
public:
    MoveMenuCommand(std::shared_ptr<Menu> menu, int x, int y);
    
    void execute() override;
};

class ZoomCommand : public Command
{
private:
    std::shared_ptr<Sim> sim;
    int scaleBefore;
    int scale;
public:
    ZoomCommand(std::shared_ptr<Sim> sim, float value)

    void execute() override;
    void undo() override;
};

class SwitchMenuCommand : public Command
{
private:
    std::shared_ptr<Sim> sim;
public:
    SwitchMenu(std::shared_ptr<Sim> sim, int menuIndex1, int menuIndex2);

    void execute() override;
};

class SelectElemCommand : public Command
{
    private:
        std::shared_ptr<Sim> sim;
        ElementType type;
    public:
        SelectElemCommand(std::shared_ptr<Sim> sim, ElementType type);

        void execute() override;
}

class InputHandler
{
    private:
        std::shared_ptr<Sim> sim;

        std::unordered_map<char, std::shared_ptr<Command>> keyMapping; // for remapping the keys;
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
        std::shared_ptr<Command> button6 = nullptr;
        std::shared_ptr<Command> buttonEqual = nullptr;
        std::shared_ptr<Command> buttonMinus = nullptr;
        bool isPressed(char c);
    public:
        InputHandler(std::shared_ptr<Sim> sim);
        ~InputHandler();

        std::shared_ptr<Command> handleInput();
};

#endif
