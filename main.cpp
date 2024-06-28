#define OLC_PGE_APPLICATION
#include <olcPixelGameEngine.h>
#include "circuit_elements.hpp"
#include "circuit.hpp"
#include "simulator.hpp"

int main() 
{
    Cable cable;
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

    std::vector<std::pair<int, int>> resolutions = { {1280, 720}, {840, 480} };

    // simulator::Start()
    for(auto resolution : resolutions)
    {
        if(sim.Construct(resolution.first, resolution.second, 1, 1))
                break;
        else
            throw StartupFailed("Failed to start application! Trying a lower resolution.");
    }

    try {
        sim.Start();
    } catch(StartupFailed&)
    {
        return 0;
    }
}
