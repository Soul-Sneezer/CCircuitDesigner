#include "circuit.hpp"
    void Circuit::selectElement(olc::PixelGameEngine* pge, float scale)
    {
        olc::vf2d mousePos = {(float)pge->GetMouseX(), (float)pge->GetMouseY()};
        std::pair<olc::vi2d, olc::vi2d> linePos;
        olc::vf2d screenPos;
        
        for(long unsigned int i = 0; i < elements.size(); i++)
        {
            olc::vf2d pos1 = elements[i]->getPosition().first;
            olc::vf2d pos2 = elements[i]->getPosition().second;

            elements[i]->WorldToScreen(pos1, linePos.first.x, linePos.first.y);
            elements[i]->WorldToScreen(pos2, linePos.second.x, linePos.second.y);
            
            screenPos.x = (linePos.first.x + linePos.second.x) / 2;
            screenPos.y = (linePos.first.y + linePos.second.y) / 2;
            
            if((pow(abs(mousePos.x - (float)screenPos.x), 2) + pow(abs(mousePos.y - (float)screenPos.y), 2)) < scale * scale / 8)
            {
                pge->DrawLine(linePos.first.x, linePos.first.y, linePos.second.x, linePos.second.y, olc::YELLOW);
            }
        } 
    }

    void Circuit::createAndAddElem(ElementType type, std::pair<olc::vf2d, olc::vf2d> linePos)
    {
        std::shared_ptr<CircuitElement> element;
        switch(type)
        {
            case ElementType::ELEM_CABLE:
                element = std::dynamic_pointer_cast<Cable>(element);
                element = std::make_shared<Cable>(linePos);
                break;
            case ElementType::ELEM_RESISTOR:
                element = std::dynamic_pointer_cast<Resistor>(element);
                element = std::make_shared<Resistor>(linePos);
                break;
            case ElementType::ELEM_TRANSISTOR:
                element = std::dynamic_pointer_cast<Transistor>(element);
                element = std::make_shared<Transistor>(linePos);
                break;
            case ElementType::ELEM_SOURCE:
                element = std::dynamic_pointer_cast<Source>(element);
                element = std::make_shared<Source>(linePos);
                break;
            case ElementType::ELEM_BATTERY:
                element = std::dynamic_pointer_cast<Battery>(element);
                element = std::make_shared<Battery>(linePos);
                break;
            default:
                throw OperationFailed("Failed to create new element!");
        }

        addElementToCircuit(element);
    }

    void Circuit::drawElement(std::shared_ptr<CircuitElement> element, olc::PixelGameEngine *pge)
    {
        element->drawYourself(pge);
    }

    void Circuit::drawCircuit(olc::PixelGameEngine *pge)
    {
        for(unsigned int i = 0; i < this->elements.size(); i++)
        {
            drawElement(this->elements[i], pge); 
        }
    }

    void swap(Circuit& first, Circuit& second) noexcept
    {
        using std::swap;
        swap(first.elements, second.elements);
        swap(first.voltageIn, second.voltageIn);
        swap(first.voltageOut, second.voltageOut);
    }

    void Circuit::addElementToCircuit(std::shared_ptr<CircuitElement>& element)
    {
        elements.push_back(element);
    }
    Circuit::Circuit()
    {
        this->voltageIn = 0;
        this->voltageOut = 0;        
    }

    Circuit::Circuit(const Circuit& circuit)
    {
        this->voltageIn = circuit.voltageIn;
        this->voltageOut = circuit.voltageOut;
        this->elements = circuit.elements;
    }

    Circuit::~Circuit()
    {

    }

    std::ostream& operator<<(std::ostream& os, const Circuit& el)
    {
        os<<"Voltage in: "<<el.getVoltageIn()<<"  Voltage out: "<<el.getVoltageOut()<<"\n";
        return os;
    }

    int32_t Circuit::getVoltageIn() const{ return this->voltageIn; };
    int32_t Circuit::getVoltageOut() const{ return this->voltageOut; };

    // cppcheck-suppress unusedFunction
    std::vector<std::shared_ptr<CircuitElement>>& Circuit::getElements() { return this->elements; };
    Circuit& Circuit::operator=(const Circuit& circuit)
    {
        this->voltageIn = circuit.voltageIn;
        this->voltageOut = circuit.voltageOut;
        this->elements = circuit.elements;

        return *this;
    }

    void Circuit::run()
    {
    }
