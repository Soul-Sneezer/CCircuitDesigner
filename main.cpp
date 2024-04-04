#include <iostream>
#include <vector>

class CircuitElement
{
private:
    std::pair<int, int> position;

    int voltage;
    int power;
    int temperature; // useless for now

    CircuitElement* in; // prev element(may be NULL)
    CircuitElement* out; // next element
public:
    CircuitElement()
    {
        position.first = 0;
        position.second = 0;
        this->voltage = 0;
        this->power = 0;
        this->temperature = 273;
        this->in = NULL;
        this->out = NULL;
    }
    
    explicit CircuitElement(const int voltage = 0, const int power = 0, \
            const CircuitElement* in = NULL, const CircuitElement* out = NULL, \
            const pair<int, int> position = std::make_pair(0, 0), const int temperature = 273)
    {
        this->voltage = voltage;
        this->power = power;
        this->temperature = temperature;
        this->in = in;
        this->out = out;
        this->position = position;
    }

    explicit CircuitElement(const CircuitElement& element)
    {
        this->position= element.position;
        this->voltage = element.voltage;
        this->power = element.power;
        this->temperature = element.temperature;
        this->in = element.in;
        this->out = element.out;
    }

    ~CircuitElement()
    {
    }

    int getVoltage() { return this->voltage; };
    int getPower() { return this->power; };
    int getTemperature() { return this->temperature; };
    CircuitElement* getPrev() { return this->in; };
    CircuitElement* getNext() { return this->out; };

    friend std::ostream& operator<<(std::ostream& os, const CircuitElement& el)
    {
        os << "Voltage: "<<el.voltage <<"    Power: " <<el.power <<"    Temperature: "<<el.temperature<<"\n";
        return os;        
    }

    CircuitElement& operator=(const CircuitElement& element)
    {
        this->position = element.position;
        this->voltage = element.voltage;
        this->power = element.power;
        this->temperature = element.temperature;
        this->in = element.in;
        this->out = element.out;

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

    explicit Transistor(const int threshold = 0, const int voltage = 0, const int power = 0, \
            const CircuitElement* in = NULL, const CircuitElement* out = NULL, \
            const pair<int, int> position = std::make_pair(0, 0), const int temperature = 273)
    {
        this->threshold = threshold;
        this->voltage = voltage;
        this->power = power;
        this->in = in;
        this->out = out;
        this->position = position;
        this->temperature = temperature;
    }

    explicit Transistor(const Transistor& transistor)
    {
        this->threshold = transistor.threshold;
        this->voltage = transistor.voltage;
        this->power = transistor.power;
        
        this->in = transistor.in;
        this->out = transistor.out;

        this->temperature = transistor.temperature;
        this->position = transistor.position;
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
        this->voltage = transistor.voltage;
        this->power = transistor.power;

        this->in = transistor.in;
        this->out = transistor.out;

        this->temperature = transistor.temperature;
        this->position = transistor.position;

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

    explicit Resistor(const int resistance = 0, const int powerDissipation = 0, const tolerance = 0, \
            const voltage = 0, const power = 0, const CircuitElement* in = NULL, const CircuitElement* out = NULL, \
            const pair<int, int> position = std::make_pair(0,0), const int temperature = 273)
    {
        this->resistance = resistance;
        this->powerDissipation = powerDissipation;
        this->tolerance = tolerance;

        this->voltage = voltage;
        
        this->power = power;
        this->in = in;
        this->out = out;
        
        this->temperature = temperature;
        this->position = position;

    }

    explicit Resistor(const Resistor& r)
    {
        this->resistance = r.resistance;
        this->powerDissipation = r.powerDissipation;
        this->tolerance = r.tolerance;

        this->voltage = r.voltage;
        
        this->power = r.power;
        this->in = r.in;
        this->out = r.out;
        
        this->temperature = r.temperature;
        this->position = r.position;
    }

    ~Resistor()
    {
    }

    friend std::ostream& operator<<(std::ostream& os, const Resistor& el)
    {
        os<<"Resistance: "<<el.resistance<<"    Power that can be safely dissipated: "<<el.powerDissipation<<"   Tolerance: "<<el.tolerance<<"\n";
        return os; 
    }

    Resistor& operator=(const Resistor& r)
    {
        this->resistance = r.resistance;
        this->powerDissipation = r.powerDissipation;
        this->tolerance = r.tolerance;

        this->voltage = r.voltage;
        this->power = r.power;
        
        this->in = r.in;
        this->out = r.out;
        
        this->temperature = r.temperature;
        this->position = r.position;
            

        return *this;
    }
};

class Cable : CircuitElement
{
private:
    std::pair<int, int> end;

    int resistance;
    bool reverse;
public:
    Cable()
    {
        this->end = std::make_pair(0,0);

        this->resistance = 0;
        this->reverse = false;
    }

    explicit Cable(const int resistance = 0, const bool reverse = false, \
            const int voltage = 0, const int power = 0, const CircuitElement* in = NULL, const CircuitElement* out = NULL, \
            const pair<int, int> start = std::make_pair(0,0), const pair<int, int> end = std::make_pair(0,0), const int temperature = 273)
    {
        this->resistance = resistance;
        this->reverse = reverse;

        this->voltage = voltage;
        this->power = power;

        this->in = in;
        this->out = out;

        this->position = start;
        this->end = end;

        this->temperature = temperature;
    }

    explicit Cable(const Cable& c)
    {
        this->position = c.position;
        this->end = c.end;
        
        this->resistance = c.resistance;
        this->reverse = c.reverse;
        
        this->power = c.power;
        this->voltage = c.voltage;
        
        this->temperature = c.temperature;
   
        this->in = c.in;
        this->out = c.out;
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
        this->position = cable.position;
        this->end = cable.end;
        
        this->reverse = cable.reverse;
        this->resistance = cable.resistance;
        
        this->voltage = cable.voltage;
        this->power = cable.power;
        
        this->temperature = cable.temperature;

        this->in = cable.in;
        this->out = cable.out;

        return *this;
    }
};

class Source : CircuitElement
{
private:
public:
    Source() : CircuitElement()
    {
    }
    
    explicit Source(const int resistance = 0, const bool reverse = false, \
            const int voltage = 0, const int power = 0, const CircuitElement* in = NULL, const CircuitElement* out = NULL, \
            const pair<int, int> start = std::make_pair(0,0), const pair<int, int> end = std::make_pair(0,0), const int temperature = 273)
    {

        this->voltage = voltage;
        this->power = power;

        this->in = in;
        this->out = out;

        this->position = start;
        this->temperature = temperature;
    }

    explicit Source(const Source& s)
    {
        this->power = s.power;
        this->voltage = s.voltage;
        
        this->in = s.in;
        this->out = s.out;

        this->position = s.position;
        this->temperature = s.temperature;
    }

    ~Source()
    {
    }

    friend std::ostream& operator<<(std::ostream& os, const Source& el)
    {
        return os;
    }

    Source& operator=(const Source& s)
    {
        this->power = s.power;
        this->voltage = s.voltage;
        
        this->in = s.in;
        this->out = s.out;

        this->position = s.position;
        this->temperature = s.temperature;


        return *this;
    }
};


class Battery
{
    private:
        std::pair<int, int> position;
        int capacity;
        int voltage;
        CircuitElement* out;
    public:
    Battery()
    {
        this->position = std::make_pair(0, 0);
        this->capacity = 0;
        this->voltage = 0;
        this->out = NULL;
    }

    explicit Battery(const int voltage = 0, const int capacity = 0, \ 
            const CircuitElement* out = NULL, const std::pair<int, int> position = std::make_pair(0,0))
    {
        this->voltage = voltage;
        this->capacity = capacity;
        this->out = out;
        this->position = position;
    }

    explicit Battery(const Battery& b)
    {
        this->position = b.position;
        this->capacity = b.capacity;
        this->voltage = b.voltage;
        this->out = b.out;
    }

    ~Battery()
    {
    }

    friend std::ostream& operator<<(std::ostream& os, const Battery& el)
    {
        return os;
    }

    Baterry& operator=(const Battery& b)
    {
        this->position = b.position;
        this->capacity = b.capacity;
        this->voltage = b.voltage;
        this->out = b.out;

        return *this;
    }
};

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
