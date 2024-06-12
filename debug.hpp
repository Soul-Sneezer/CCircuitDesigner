#ifndef CC_DEBUG
#define CC_DEBUG

class Error : public std::runtime_error
{
    public:
        explicit Error(const char* message) throw() : std::runtime_error(message)
    {}
};

class OperationFailed : public Error
{
    public:
        explicit OperationFailed(const char* message) : Error(message)
    {}
};

class StartupFailed : public Error
{
    public:
        explicit StartupFailed(const char* message) : Error(message)
    {}
};

class SpriteAllocFailed : public Error
{
    public:
        explicit SpriteAllocFailed(const char* message) : Error(message)
    {}
};

#endif
