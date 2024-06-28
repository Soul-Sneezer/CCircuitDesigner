#include "circuit.hpp"

// cppcheck-suppress unusedFunction
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

    void Circuit::addElem(std::shared_ptr<CircuitElement>& element)
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
        
        for (const auto& element : circuit.elements)
            this->elements.push_back((std::shared_ptr<CircuitElement>)element->clone());
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

    Circuit& Circuit::operator=(Circuit& circuit)
    {
        this->voltageIn = circuit.voltageIn;
        this->voltageOut = circuit.voltageOut;
        swap(*this, circuit); 

        return *this;
    }

    void Circuit::run()
    {
    }
