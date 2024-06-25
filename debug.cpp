#include "debug.hpp"

Error::Error(const char* message) throw() : std::runtime_error(message)
{
}

OperationFailed::OperationFailed(const char* message) : Error(message)
{

}

StartupFailed::StartupFailed(const char* message) : Error(message)
{

}

SpriteAllocFailed::SpriteAllocFailed(const char* message) : Error(message)
{

}
