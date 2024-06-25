#ifndef CC_DEBUG
#define CC_DEBUG

#include <stdexcept> 

class Error : public std::runtime_error
{
    public:
        explicit Error(const char* message) throw(); 
};

class OperationFailed : public Error
{
    public:
        explicit OperationFailed(const char* message);
};

class StartupFailed : public Error
{
    public:
        explicit StartupFailed(const char* message); 
};

class SpriteAllocFailed : public Error
{
    public:
        explicit SpriteAllocFailed(const char* message); 
};

#endif
