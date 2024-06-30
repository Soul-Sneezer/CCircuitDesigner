#define OLC_PGE_APPLICATION
#include "engine.hpp"

int main() 
{

    Engine engine;
    if(!engine.Construct(1280, 720, 1, 1))
        throw StartupFailed("Failed to start application!");

    try {
        engine.Start();
    } catch(StartupFailed&)
    {
        return 0;
    }
}
