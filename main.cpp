#include <iostream>
#include <vector>

class CircuitElement
{
private:
    int powerIn;
    int powerOut;
public:
    CircuitElement()
    {
        this->powerIn = 0;
        this->powerOut = 0;
    }

    CircuitElement(int value)
    {
        this->powerIn = value;
        this->powerOut = value;
    }

    CircuitElement(int in, int out)
    {
        this->powerIn = in;
        this->powerOut = out;
    }

    CircuitElement(const CircuitElement& element)
    {
        this->powerIn = element.powerIn;
        this->powerOut = element.powerOut;
    }

    ~CircuitElement()
    {
    }

    int getPowerOut() { return this->powerOut; };

    friend std::ostream& operator<<(std::ostream& os, const CircuitElement& el)
    {
        os << "Power in: "<<el.powerIn <<"    Power out: " <<el.powerOut <<"\n";
        return os;        
    }

    CircuitElement& operator=(const CircuitElement& element)
    {
        this->powerIn = element.powerIn;
        this->powerOut = element.powerOut;

        return *this;
    }

    virtual void changeValue()
    {
    }
};

class Transistor : CircuitElement
{
private:
    int threshold;
public:
    Transistor()
    {
        this->threshold = 0;
    }

    Transistor(int threshold) 
    {
        this->threshold = threshold;
    }

    Transistor(const Transistor& transistor)
    {
        this->threshold = transistor.threshold;
    }

    ~Transistor()
    {
    }

    friend std::ostream& operator<<(std::ostream& os, const Transistor& el)
    {
        os<<"Threshold value: "<<el.threshold<<"\n";
        return os;
    }

    Transistor& operator=(const Transistor& transistor)
    {
        this->threshold = transistor.threshold;
        return *this;
    }
};

class Resistor : CircuitElement
{
private:
    int resistance;
public:
    Resistor()
    {
    }

    Resistor(int resistance) 
    {
        this->resistance = resistance;
    }

    Resistor(const Resistor& r)
    {
        this->resistance = r.resistance;
    }

    ~Resistor()
    {
    }

    friend std::ostream& operator<<(std::ostream& os, const Resistor& el)
    {
        os<<"Resistance: "<<el.resistance<<"\n";
        return os;
    }

    Resistor& operator=(const Resistor& resistor)
    {
        this->resistance = resistor.resistance;
        return *this;
    }
};

class Cable : CircuitElement
{
private:
    std::pair<int, int> start;
    std::pair<int, int> end;

    bool reverse;
public:
    Cable()
    {
    }

    Cable(std::pair<int, int> start, std::pair<int, int> end) 
    {
        this->start = start;
        this->end = end;
    }

    Cable(const Cable& c)
    {

    }

    ~Cable()
    {
    }

    friend std::ostream& operator<<(std::ostream& os, const Cable& el)
    {
        
        return os;
    }

    Cable& operator=(const Cable& cable)
    {
        this->start = cable.start;
        this->end = cable.end;
        this->reverse = cable.reverse;

        return *this;
    }
};

class Source : CircuitElement
{
private:
public:
    Source()
    {
    }
    
    Source(int power)  : CircuitElement(power)
    {
    }

    Source(const Source& s)
    {
    }

    ~Source()
    {
    }

    friend std::ostream& operator<<(std::ostream& os, const Source& el)
    {
        return os;
    }

    Source& operator=(const Source& source)
    {
        return *this;
    }
};

class Circuit
{
private:
    int powerIn;
    int powerOut;

    std::vector<CircuitElement> elements; // not suitable for a circuit, replace soon
public:
    Circuit()
    {

    }

    Circuit(const Circuit& circuit)
    {
        this->elements = circuit.elements;
    }

    ~Circuit()
    {

    }

    friend std::ostream& operator<<(std::ostream& os, const Circuit& el)
    {
        return os;
    }

    Circuit& operator=(const Circuit& circuit)
    {
        this->elements = circuit.elements;

        return *this;
    }
    
    std::vector<CircuitElement>& getElements() { return this->elements; };

    void addElemToCircuit(CircuitElement* element)
    {
        getElements().push_back(*element);
    }

    void removeElemFromCircuit(const int index)
    {
        std::vector<CircuitElement>& v = getElements();
        v.erase(v.begin() + index);
    }

    int output()
    {
        
        return 0;
    }
};

int main() {
}
