#ifndef CC_ENGINE
#define CC_ENGINE

#include "simulator.hpp"
#include "command.hpp"

class Engine : public olc::PixelGameEngine
{
private: 
    std::shared_ptr<Sim> sim;
    std::shared_ptr<InputHandler> inputHandler;
public:
    Engine();
    // cppcheck-suppress unusedFunction
    [[maybe_unused]] bool OnUserCreate() override;
    // cppcheck-suppress unusedFunction
    [[maybe_unused]] bool OnUserUpdate([[maybe_unused]] float fElapsedTime) override;
};

#endif
