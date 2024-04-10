#include <iostream>
#include <vector>

//#define OLC_PGE_APPLICATION
//#include "olcPixelGameEngine.h"

class CircuitElement
{
protected:
    std::pair<int, int> position;

    int voltage;
    int power;
    int temperature; // useless for now

    CircuitElement* in; // prev element
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
    
    explicit CircuitElement(const int voltage, const int power = 0, \
            CircuitElement* in = NULL, CircuitElement* out = NULL, \
            const std::pair<int, int> position = std::make_pair(0, 0), const int temperature = 273)
    {
        this->voltage = voltage;
        this->power = power;
        this->temperature = temperature;
        this->in = in;
        this->out = out;
        this->position = position;
    }

    explicit CircuitElement(CircuitElement& element)
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
    
    std::pair<int, int> getPosition() { return this->position; };
    int getVoltage() { return this->voltage; };
    int getPower() { return this->power; };
    int getTemperature() { return this->temperature; };
    CircuitElement* getIn() { return this->in; };
    CircuitElement* getOut() { return this->out; };

    friend std::ostream& operator<<(std::ostream& os, CircuitElement& el)
    {
        os << "Voltage: "<<el.voltage <<"    Power: " <<el.power <<"    Temperature: "<<el.temperature<<"\n";
        return os;        
    }

    CircuitElement& operator=(CircuitElement& element)
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

class CableNode : public CircuitElement
{
private:
    std::vector<CircuitElement*> inputs;
    std::vector<CircuitElement*> outputs;
public:
    CableNode()   
    {
    }

    explicit CableNode(std::vector<CircuitElement*>& in, std::vector<CircuitElement*>& out)
    {
       this->inputs = in; 
       this->outputs = out;
    }

    ~CableNode()
    {
        
    }

    friend std::ostream& operator<<(std::ostream& os, CableNode& el)
    {
        os<<"Cable node\n";
        return os;        
    }

    std::vector<CircuitElement*> getIn() { return this->inputs; };
    std::vector<CircuitElement*> getOut() { return this->outputs; };

    CableNode& operator=(CableNode& element)
    {
        this->inputs = element.getIn();
        this->outputs = element.getOut();

        return *this;
    }
};

class Transistor : public CircuitElement
{
private:
    int thresholdVoltage;
    int threshold;
public:
    Transistor() 
    {
        this->threshold = 0;
        this->thresholdVoltage = 0;
    }

    explicit Transistor(const int threshold, const int voltage = 0, const int power = 0, \
            CircuitElement* in = NULL, CircuitElement* out = NULL, const int thresholdVoltage = 0, \
            const std::pair<int, int> position = std::make_pair(0, 0), const int temperature = 273)
    {
        this->threshold = threshold;
        this->thresholdVoltage = thresholdVoltage;
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
        this->thresholdVoltage = transistor.thresholdVoltage;
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
        os<<"Threshold value: "<<el.threshold<<"    Threshold voltage: "<<el.thresholdVoltage<<"\n";
        os << "Voltage: "<<el.voltage <<"    Power: " <<el.power <<"    Temperature: "<<el.temperature<<"\n";

        return os;
    }

    int getThreshold() { return this->threshold; };
    int getThresholdVoltage() { return this->thresholdVoltage; };

    Transistor& operator=(Transistor& transistor)
    {
        this->threshold = transistor.getThreshold();
        this->voltage = transistor.getVoltage();
        this->power = transistor.getPower();

        this->in = transistor.getIn();
        this->out = transistor.getOut();

        this->temperature = transistor.getTemperature();
        this->position = transistor.getPosition();

        return *this;
    }
};

class Resistor : public CircuitElement
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

    explicit Resistor(const int resistance = 0, const int powerDissipation = 0, const int tolerance = 0, \
            const int voltage = 0, const int power = 0, CircuitElement* in = NULL, CircuitElement* out = NULL, \
            const std::pair<int, int> position = std::make_pair(0,0), const int temperature = 273)
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
        os << "Voltage: "<<el.voltage <<"    Power: " <<el.power <<"    Temperature: "<<el.temperature<<"\n";

        return os; 
    }

    int getResistance() { return this->resistance; };
    int getPowerDissipation() { return this->powerDissipation; };
    int getTolerance() { return this->tolerance; };

    Resistor& operator=(Resistor& r)
    {
        this->resistance = r.getResistance();
        this->powerDissipation = r.getPowerDissipation();
        this->tolerance = r.getTolerance();

        this->voltage = r.getVoltage();
        this->power = r.getPower();
        
        this->in = r.getIn();
        this->out = r.getOut();
        
        this->temperature = r.getTemperature();
        this->position = r.getPosition();
            

        return *this;
    }
};

class Switch
{
    private:
        bool open;
        bool active;
    public:
    Switch()
    {
        open = false;
        active = false; // determines if the switch is drawn or not
    }

    ~Switch()
    {
    }
    
    bool isOpen()
    {
        return open;
    }

    bool isActive()
    {
        return active;
    }

    void activate()
    {
        this->active = true;
    }

    void deactivate()
    {
        this->active = false;
    }

    void openSwitch()
    {
        this->open = true;
    }

    void closeSwitch()
    {
        this->open = false;
    }
};

class Cable : public CircuitElement
{
private:
    std::pair<int, int> end;
    int resistance;
    bool reverse;

    Switch circuitSwitch;
public:
    Cable()
    {
        this->end = std::make_pair(0,0);

        this->resistance = 0;
        this->reverse = false;
    }

    explicit Cable(const int voltage, const int power = 0, CircuitElement* in = NULL,CircuitElement* out = NULL, \
            const std::pair<int, int> start = std::make_pair(0,0), const std::pair<int, int> end = std::make_pair(0,0), const int resistance = 0, const bool reverse = false, const int temperature = 273)
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
        os <<"Resistance: "<<el.resistance<<"   Voltage: "<<el.voltage<<"    Power: " <<el.power<<"    Temperature: "<<el.temperature<<"\n";

        return os;
    }

    std::pair<int, int> getEnd() { return this->end; };
    int getResistance() { return this->resistance; };
    Switch getSwitch() { return this->circuitSwitch; };
    bool getFlowDirection() { return this->reverse; };

    Cable& operator=(Cable& cable)
    {
        this->position = cable.getPosition();
        this->end = cable.getEnd();
        
        this->reverse = cable.getFlowDirection();
        this->resistance = cable.getResistance();
        
        this->voltage = cable.getVoltage();
        this->power = cable.getPower();
        
        this->temperature = cable.getTemperature();

        this->in = cable.getIn();
        this->out = cable.getOut();

        return *this;
    }
};

class Source : public CircuitElement
{
private:
public:
    Source()
    {
    }

    explicit Source(const Source& s)
    {
    }

    ~Source()
    {
    }

    friend std::ostream& operator<<(std::ostream& os, const Source& el)
    {
        os <<"Voltage: "<<el.voltage <<"    Power: " <<el.power <<"    Temperature: "<<el.temperature<<"\n";

        return os;
    }

    Source& operator=(Source& s)
    {
        this->power = s.getPower();
        this->voltage = s.getVoltage();
        
        this->in = s.getIn();
        this->out = s.getOut();

        this->position = s.getPosition();
        this->temperature = s.getTemperature();


        return *this;
    }
};


class Battery : public CircuitElement
{
    private:
        int capacity;
    public:
    Battery()
    {
        this->capacity = 0;
    }

    explicit Battery(const int voltage, const int power = 0, \
        CircuitElement* in = NULL, CircuitElement* out = NULL, \
        const std::pair<int, int> position = std::make_pair(0,0), \
        const int temperature = 273, const int capacity = 0)
    {
        this->voltage = voltage;
        this->power = 0;
        this->in = in;
        this->out = out;
        this->position = position;
        this->capacity = capacity;
    }

    explicit Battery(const Battery& b)
    {
        this->position = b.position;
        this->capacity = b.capacity;
        this->voltage = b.voltage;
        this->out = b.out;
        this->in = b.in;
        this->power = b.power;
        this->temperature = b.temperature;
    }

    ~Battery()
    {
    }

    friend std::ostream& operator<<(std::ostream& os, const Battery& el)
    {
        os<<"Capacity: "<<el.capacity<<" Voltage: "<<el.voltage<<"\n";
        return os;
    }
    int getCapacity() { return this->capacity; };

    Battery& operator=(Battery& b)
    {
        this->position = b.getPosition();
        this->capacity = b.getCapacity();
        this->voltage = b.getVoltage();
        this->out = b.getOut();
        this->in = b.getIn();
        this->temperature = b.getTemperature();
        this->power = b.getPower();

        return *this;
    }
};

class Circuit
{
private:
    int voltageIn;
    int voltageOut;
    std::vector<CircuitElement*> elements;
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
        os<<"Voltage in: "<<el.voltageIn<<"  Voltage out: "<<el.voltageOut<<"\n";
        return os;
    }

    CircuitElement* getIn() { return this->in; };
    CircuitElement* getOut() { return this->out; };
    int getVoltageIn() { return this->voltageIn; };
    int getVoltageOut() { return this->voltageOut; };

    Circuit& operator=(Circuit& circuit)
    {
        this->in = circuit.getIn();
        this->out = circuit.getOut();
        this->voltageIn = circuit.getVoltageIn();
        this->voltageOut = circuit.getVoltageOut();
        
        return *this;
    }
   
    void run()
    {
    }
};


/*
class Sim : public olc::PixelGameEngine
{
public:
	Sim()
	{
		sAppName = "Circuit Simulator";
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
				Draw(x, y, olc::Pixel(rand() % 255, rand() % 255, rand()% 255));	
		return true;
	}
};
*/
int main() 
{

}
