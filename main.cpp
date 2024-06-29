#define OLC_PGE_APPLICATION
#include <olcPixelGameEngine.h>
#include "circuit_elements.hpp"
#include "circuit.hpp"
#include "simulator.hpp"

int main() 
{
    Sim sim;

    if(!sim.Construct(1280, 720, 1, 1))
        throw StartupFailed("Failed to start application! Trying a lower resolution.");


    try {
        sim.Start();
    } catch(StartupFailed&)
    {
        return 0;
    }
}
