#define OLC_PGE_APPLICATION
#include <olcPixelGameEngine.h>
#include "circuit_elements.hpp"
#include "circuit.hpp"
#include "simulator.hpp"

int main() 
{
    int value = 3;
    Cable cable;
    cable.changeValue(value);
    Switch circuit_switch = cable.getSwitch();
    circuit_switch.activate();
    circuit_switch.deactivate();
    circuit_switch.isActive();
    circuit_switch.isOpen();
    circuit_switch.openSwitch();
    circuit_switch.closeSwitch();
    Circuit circuit;
    circuit.run();


    Transistor transistor;
    transistor.getThreshold();
    transistor.getThresholdVoltage();

    Sim sim;
    try{
        if(sim.Construct(1280, 720, 1, 1))
            sim.Start();
        else
            throw StartupFailed("Failed to start simulation! Trying at a lower resolution.");
    }
    catch(StartupFailed const &)
    {
        // try starting the sim at a lower resolution
        try{if(sim.Construct(640, 480, 1, 1))
            sim.Start();
        else
            throw StartupFailed("Failed to start simulation! Closing program.");
        }
        catch(StartupFailed const &)
        {
            return 0;
        }
    }
}
