#include "engine.hpp"

Engine::Engine()
{
    sAppName = "Circuit Simulator";
}

// cppcheck-suppress unusedFunction
    [[maybe_unused]] bool Engine::OnUserCreate()
    {
        allocSprites();

        sim = std::make_shared<Sim>(); 
        inputHandler = std::make_shared<InputHandler>();

        sim->initializeWorldOffset(this);
        sim->resetTempCoord(this);

        return true;
    }

    // cppcheck-suppress unusedFunction
    [[maybe_unused]] bool Engine::OnUserUpdate([[maybe_unused]] float fElapsedTime)
    {
        if(sim->getExitStatus())
            return false;
        Clear(olc::BLACK);

        CircuitElement::setWorldOffset(sim->getWorldOffset());
        CircuitElement::setWorldScale(sim->getScale());

        sim->drawGrid(this);
        
        try{
            sim->addElem(this);
        } 
        catch (OperationFailed const &)
        {
            sim->setAddElementFalse(); 
        }

        sim->drawCircuit(this); 
        sim->drawMenus(this);
        inputHandler->handleInput(sim.get(), this);

        return true;
    }
