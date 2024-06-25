#include "circuit_factory.hpp"

std::shared_ptr<CircuitElement> Factory::makeElement(const ElementType type, const std::pair<olc::vf2d, olc::vf2d> linePos)
{
    std::shared_ptr<CircuitElement> element;
    switch(type)
    {
        case ElementType::ELEM_CABLE:
            element = makeCable(linePos);
            break;
        case ElementType::ELEM_RESISTOR:
            element = makeResistor(linePos);
            break;
        case ElementType::ELEM_TRANSISTOR:
            element = makeTransistor(linePos); 
            break;
        case ElementType::ELEM_SOURCE:
            element = makeSource(linePos);
            break;
        case ElementType::ELEM_BATTERY:
            element = makeBattery(linePos);
            break;
        default:
            throw OperationFailed("Failed to create new element!");
            return nullptr;
    }
    return element;
}

std::shared_ptr<Transistor> Factory::makeTransistor(const std::pair<olc::vf2d, olc::vf2d> linePos) 
{
    return std::make_shared<Transistor>(linePos);
}

std::shared_ptr<Resistor> Factory::makeResistor(const std::pair<olc::vf2d, olc::vf2d> linePos) 
{
    return std::make_shared<Resistor>(linePos);
}

std::shared_ptr<Source> Factory::makeSource(const std::pair<olc::vf2d, olc::vf2d> linePos) 
{
    return std::make_shared<Source>(linePos);
}

std::shared_ptr<Battery> Factory::makeBattery(const std::pair<olc::vf2d, olc::vf2d> linePos) 
{
    return std::make_shared<Battery>(linePos);
}

std::shared_ptr<Cable> Factory::makeCable(const std::pair<olc::vf2d, olc::vf2d> linePos) 
{
    return std::make_shared<Cable>(linePos);
}
