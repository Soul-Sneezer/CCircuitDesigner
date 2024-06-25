#ifndef CC_CIRCUIT_FACTORY
#define CC_CIRCUIT_FACTORY

#include "circuit_elements.hpp"
#include "debug.hpp"

class Factory
{
public:
    Factory() = default;
    ~Factory() = default;

    std::shared_ptr<CircuitElement> makeElement(const ElementType type, const std::pair<olc::vf2d, olc::vf2d> linePos);
    std::shared_ptr<Transistor> makeTransistor(const std::pair<olc::vf2d, olc::vf2d> linePos);
    std::shared_ptr<Resistor> makeResistor(const std::pair<olc::vf2d, olc::vf2d> linePos);
    std::shared_ptr<Source> makeSource(const std::pair<olc::vf2d, olc::vf2d> linePos);
    std::shared_ptr<Battery> makeBattery(const std::pair<olc::vf2d, olc::vf2d> linePos);
    std::shared_ptr<Cable> makeCable(const std::pair<olc::vf2d, olc::vf2d> linePos);
};

#endif
