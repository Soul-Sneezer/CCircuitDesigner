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

class MoveMenuCommand : public Command
{
private:
    std::shared_ptr<Menu> menu;
    int xBefore, yBefore;
    int x, y;
public:
    MoveMenuCommand(std::shared_ptr<Menu> menu, int x, int y);
    
    void execute() override;
    void undo() override;
};

class ZoomCommand : public Command
{
private:
    std::shared_ptr<Sim> sim;
    int scaleBefore;
    int scale;
public:
    ZoomCommand(std::shared_ptr<Sim> sim, float value)
    {
        this->sim = sim;
        this->scaleBefore = sim->scale;
        this->scale = value;
    }

    void execute() override;
    void undo() override;
};

class InputHandler
{
    private:
        
        std::unordered_map<char, std::shared_ptr<Command>> keyMapping; // for remapping the keys;
        std::shared_ptr<Command> buttonW;
        std::shared_ptr<Command> buttonA;
        std::shared_ptr<Command> buttonS;
        std::shared_ptr<Command> buttonD;
        std::shared_ptr<Command> buttonE;
        std::shared_ptr<Command> buttonR;
        std::shared_ptr<Command> buttonX;
        std::shared_ptr<Command> button1;
        std::shared_ptr<Command> button2;
        std::shared_ptr<Command> button3;
        std::shared_ptr<Command> button4;
        std::shared_ptr<Command> button5;
        std::shared_ptr<Command> button6;
        std::shared_ptr<Command> buttonEqual;
        std::shared_ptr<Command> buttonMinus;
        bool isPressed(char);
    public:
        InputHandler();
        ~InputHandler();

        std::shared_ptr<Command> handleInput();
};

#endif
