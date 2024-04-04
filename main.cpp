#include <iostream>
#include <vector>

class CircuitElement
{
private:
    std::pair<int, int> position;

    int voltageIn;
    int voltageOut;
    int temperature; // useless for now

    CircuitElement* in; // prev element(may be NULL)
    CircuitElement* out; // next element
public:
    CircuitElement()
    {
        this->voltageIn = 0;
        this->voltageOut = 0;
        this->in = NULL;
        this->out = NULL;
    }

    CircuitElement(int value)
    {
        this->voltageIn = value;
        this->voltageOut = value;
    }

    CircuitElement(int in, int out)
    {
        this->voltageIn = in;
        this->voltageOut = out;
    }

    CircuitElement(const CircuitElement& element)
    {
        this->voltageIn = element.powerIn;
        this->voltageOut = element.powerOut;
    }

    ~CircuitElement()
    {
    }

    int getPowerOut() { return this->voltageOut; };

    friend std::ostream& operator<<(std::ostream& os, const CircuitElement& el)
    {
        os << "Power in: "<<el.voltageIn <<"    Power out: " <<el.powerOut <<"\n";
        return os;        
    }

    CircuitElement& operator=(const CircuitElement& element)
    {
        this->voltageIn = element.powerIn;
        this->voltageOut = element.powerOut;

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
    int powerDissipation;
    int tolerance;
public:
    Resistor()
    {
        this->resistance = 0;
        this->powerDissipation = 0;
        this->tolerance = 0;
    }

    Resistor(int resistance) 
    {
        this->resistance = resistance;
        this->powerDissipation = 0;
        this->tolerance = 0;
    }

    Resistor(const Resistor& r)
    {
        this->resistance = r.resistance;
        this->powerDissipation = r.powerDissipation;
        this->tolerance = tolerance;
    }

    ~Resistor()
    {
    }

    friend std::ostream& operator<<(std::ostream& os, const Resistor& el)
    {
        os<<"Resistance: "<<el.resistance<<"    Power that can be safely dissipated: "<<el.powerDissipation<<"   Tolerance: "<<el.tolerance<<"\n";
        return os; 
    }

    Resistor& operator=(const Resistor& resistor)
    {
        this->resistance = resistor.resistance;
        this->powerDissipation = resistor.powerDissipation;
        this->tolerance = resistor.tolerance;
        return *this;
    }
};

class Cable : CircuitElement
{
private:
    std::pair<int, int> start;
    std::pair<int, int> end;

    int resistance;
    bool reverse;
public:
    Cable()
    {
        this->start.first = 0;
        this->start.second = 0;

        this->end.first = 0;
        this->end.second = 0;

        this->resistance = 0;
        this->reverse = 0;
    }

    Cable(std::pair<int, int> start, std::pair<int, int> end) 
    {
        this->start = start;
        this->end = end;
    }

    Cable(const Cable& c)
    {
        this->start = c.start;
        this->end = c.end;
        this->resistance = c.resistance;
        this->reverse = c.reverse;
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
        this->resistance = cable.resistance;

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
    
    Source(int voltage)  : CircuitElement(voltage)
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


class Battery
{
    private:
    public:
    Battery()
    {
    }

    Baterry(const Battery& b)
    {
    }

    ~Battery()
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
}

class Circuit
{
private:
    int voltageIn;
    int voltageOut;

    CircuitElement* in;
    CircuitElement* out;
public:
    Circuit()
    {
        this->voltageIn = 0;
        this->voltageOut = 0;        
        this->in = NULL;
        this->out = NULL;
    }

    Circuit(const Circuit& circuit)
    {
        this->in = circuit.in;
        this->out = circuit.out;
        this->voltageIn = circuit.voltageIn;
        this->voltageOut = circuit.voltageOut;
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
        return *this;
    }
    
    std::vector<CircuitElement>& getElements() { return this->elements; };

    void addElemToCircuit(CircuitElement* element)
    {
    }

    void newInputElem(CircuitElement* element)
    {
        this->in = element;
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
