#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class CircuitElement
{
protected:
    std::pair<int32_t, int32_t> position;

    int32_t voltage;
    int32_t power;
    uint32_t temperature; // useless for now

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
    
    explicit CircuitElement(const int32_t voltage, const int32_t power = 0, \
            CircuitElement* in = NULL, CircuitElement* out = NULL, \
            const std::pair<int32_t, int32_t> position = std::make_pair(0, 0), const int32_t temperature = 273)
    {
        this->voltage = voltage;
        this->power = power;
        this->temperature = temperature;
        this->in = in;
        this->out = out;
        this->position = position;
    }

    CircuitElement(const CircuitElement& element)
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
    
    std::pair<int32_t, int32_t> getPosition() { return this->position; };
    int32_t getVoltage() { return this->voltage; };
    int32_t getPower() { return this->power; };
    int32_t getTemperature() { return this->temperature; };
    CircuitElement* getIn() { return this->in; };
    CircuitElement* getOut() { return this->out; };

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

class CableNode : public CircuitElement
{
private:
    std::vector<CircuitElement*> inputs;
    std::vector<CircuitElement*> outputs;
public:
    CableNode()   
    {
    }

    explicit CableNode(const std::vector<CircuitElement*>& in, const std::vector<CircuitElement*>& out)
    {
       this->inputs = in; 
       this->outputs = out;
    }
    
    CableNode(const CableNode& node) : CircuitElement(node)
    {
        this->inputs = node.inputs;
        this->outputs = node.outputs;
    }

    ~CableNode()
    {
        
    }

    friend std::ostream& operator<<(std::ostream& os, CableNode& el)
    {
        os<<el.inputs.size()<<" "<<el.outputs.size();
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
    int32_t thresholdVoltage;
    int32_t threshold;
public:
    Transistor() 
    {
        this->threshold = 0;
        this->thresholdVoltage = 0;
    }

    explicit Transistor(const int32_t threshold, const int32_t voltage = 0, const int32_t power = 0, \
            CircuitElement* in = NULL, CircuitElement* out = NULL, const int32_t thresholdVoltage = 0, \
            const std::pair<int32_t, int32_t> position = std::make_pair(0, 0), const int32_t temperature = 273)
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

    Transistor(const Transistor& transistor) : CircuitElement(transistor)
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

    int32_t getThreshold() { return this->threshold; };
    int32_t getThresholdVoltage() { return this->thresholdVoltage; };

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
    int32_t resistance;
    int32_t powerDissipation;
    int32_t tolerance;
public:
    Resistor()
    {
        this->resistance = 0;
        this->powerDissipation = 0;
        this->tolerance = 0;
    }

    explicit Resistor(const int32_t resistance = 0, const int32_t powerDissipation = 0, const int32_t tolerance = 0, \
            const int32_t voltage = 0, const int32_t power = 0, CircuitElement* in = NULL, CircuitElement* out = NULL, \
            const std::pair<int32_t, int32_t> position = std::make_pair(0,0), const int32_t temperature = 273)
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

    Resistor(const Resistor& r) : CircuitElement(r)
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

    int32_t getResistance() { return this->resistance; };
    int32_t getPowerDissipation() { return this->powerDissipation; };
    int32_t getTolerance() { return this->tolerance; };

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

    Switch(const Switch& s) 
    {
        this->open = s.open;
        this->active = s.active;
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
    std::pair<int32_t, int32_t> end;
    int32_t resistance;
    bool reverse;
    int32_t length;
    Switch circuitSwitch;
public:
    Cable()
    {
        this->end = std::make_pair(0,0);
        this->length = 0;
        this->resistance = 0;
        this->reverse = false;
    }

    explicit Cable(const int32_t voltage, const int32_t power = 0, CircuitElement* in = NULL,CircuitElement* out = NULL, \
            const std::pair<int32_t, int32_t> start = std::make_pair(0,0), const std::pair<int32_t, int32_t> end = std::make_pair(0,0), const int32_t resistance = 0, const bool reverse = false, const int32_t length = 0, const uint32_t temperature = 273)
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
        this->length = length;
    }

    Cable(const Cable& c) : CircuitElement(c)
    {
        this->position = c.position;
        this->end = c.end;
        
        this->resistance = c.resistance;
        this->reverse = c.reverse;
        
        this->power = c.power;
        this->voltage = c.voltage;
        
        this->temperature = c.temperature;
        this->length = c.length;

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

    std::pair<int32_t, int32_t> getEnd() { return this->end; };
    int32_t getResistance() { return this->resistance; };
    Switch getSwitch() { return this->circuitSwitch; };
    bool getFlowDirection() { return this->reverse; };
    int32_t getLength() { return this->length; };
    Cable& operator=(Cable& cable)
    {
        this->position = cable.getPosition();
        this->end = cable.getEnd();
        
        this->reverse = cable.getFlowDirection();
        this->resistance = cable.getResistance();
        
        this->voltage = cable.getVoltage();
        this->power = cable.getPower();
        
        this->temperature = cable.getTemperature();
        this->length = cable.getLength();

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
/*
    Source(const Source& s) : CircuitElement(s)
    {
    }
*/
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
        int32_t capacity;
    public:
    Battery()
    {
        this->capacity = 0;
    }

    explicit Battery(const int32_t voltage, const int32_t power = 0, \
        CircuitElement* in = NULL, CircuitElement* out = NULL, \
        const std::pair<int32_t, int32_t> position = std::make_pair(0,0), \
        const int32_t temperature = 273, const int32_t capacity = 0)
    {
        this->voltage = voltage;
        this->power = power;
        this->in = in;
        this->out = out;
        this->position = position;
        this->capacity = capacity;
        this->temperature = temperature;
    }

    Battery(const Battery& b) : CircuitElement(b)
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
    int32_t getCapacity() { return this->capacity; };

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
    int32_t voltageIn;
    int32_t voltageOut;
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
    int32_t getVoltageIn() { return this->voltageIn; };
    int32_t getVoltageOut() { return this->voltageOut; };

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

class Sim : public olc::PixelGameEngine
{
private:
    bool mainMenuActive;
    bool editMenuActive;
    bool removeMenuActive;
    bool addMenuActive;
    bool modifyMenuActive;
    int32_t menuOffsetX;
    int32_t menuOffsetY;

    void resetOffset()
    {
        menuOffsetX = 0;
        menuOffsetY = 0;
    }

    void pressEntry(const int posX, const int posY, std::string key, std::string action)
    {
        DrawString(posX, posY, "Press ");
        DrawString(posX + 45, posY, key, olc::GREEN);
        DrawString(posX + 50, posY, " to ");
        DrawString(posX + 80, posY, action);
    }
public:
	Sim()
	{
		sAppName = "Circuit Simulator";
	}

public:
	bool OnUserCreate() override
	{
    mainMenuActive = true;
    editMenuActive = false;
    addMenuActive = false;
    removeMenuActive = false;
    modifyMenuActive = false;
    menuOffsetX = 0;
    menuOffsetY = 0;
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
    Clear(olc::BLACK);
    if(mainMenuActive)
    {
        DrawRect(50 + menuOffsetX, 50 + menuOffsetY, 500, 200, olc::WHITE);
        DrawString(70 + menuOffsetX, 70 + menuOffsetY, "CCircuit Designer - Commands", olc::WHITE, 2);
        DrawString(70 + menuOffsetX, 90 + menuOffsetY, "Press");
        DrawString(115 + menuOffsetX, 90 + menuOffsetY, "ESC", olc::GREEN);
        DrawString(145 + menuOffsetX, 90 + menuOffsetY, "to open/close this window");
        pressEntry(70 + menuOffsetX, 110 + menuOffsetY, "Q", "open edit menu");
        pressEntry(70 + menuOffsetX, 130 + menuOffsetY, "R", "start/stop simulation");
        pressEntry(70 + menuOffsetX, 150 + menuOffsetY, "W", "move window up");
        pressEntry(70 + menuOffsetX, 160 + menuOffsetY, "S", "move window down");
        pressEntry(70 + menuOffsetX, 170 + menuOffsetY, "A", "move window left");
        pressEntry(70 + menuOffsetX, 180 + menuOffsetY, "D", "move window right");
    }
    if(addMenuActive)
    {
        DrawRect(50 + menuOffsetX, 50 + menuOffsetY, 500, 200, olc::WHITE);
        DrawString(70 + menuOffsetX, 70 + menuOffsetY, "Edit circuit", olc::WHITE, 2);
        pressEntry(70 + menuOffsetX, 90 + menuOffsetY, "Q", "go back to edit menu");
        pressEntry(70 + menuOffsetX, 110 + menuOffsetY, "1", "add cable");
        pressEntry(70 + menuOffsetX, 130 + menuOffsetY, "2", "add cable node");
        pressEntry(70 + menuOffsetX, 150 + menuOffsetY, "3", "add resistor");
        pressEntry(70 + menuOffsetX, 170 + menuOffsetY, "4", "add transistor");
        pressEntry(70 + menuOffsetX, 190 + menuOffsetY, "5", "add source");
        pressEntry(70 + menuOffsetX, 210 + menuOffsetY, "6", "add battery");

    }
    if(editMenuActive)
    {
        DrawRect(50 + menuOffsetX, 50 + menuOffsetY, 500, 200, olc::WHITE);
        DrawString(70 + menuOffsetX, 70 + menuOffsetY, "Edit circuit", olc::WHITE, 2);
        DrawString(70 + menuOffsetX, 90 + menuOffsetY, "Press");
        DrawString(115 + menuOffsetX, 90 + menuOffsetY, "Q", olc::GREEN);
        DrawString(126 + menuOffsetX, 90 + menuOffsetY, "to open/close this window");
        pressEntry(70 + menuOffsetX, 110 + menuOffsetY, "1","add elements to circuit");
        pressEntry(70 + menuOffsetX, 130 + menuOffsetY, "2","remove elements from circuit");
        pressEntry(70 + menuOffsetX, 150 + menuOffsetY, "3","edit elements in circuit");
        if(GetKey(olc::K1).bPressed)
        {
            addMenuActive = true;
            editMenuActive = false;
        }
        if(GetKey(olc::K2).bPressed)
        {
            removeMenuActive = true;
            editMenuActive = false;
        }
        if(GetKey(olc::K3).bPressed)
        {
            modifyMenuActive = true;
            editMenuActive = false;
        }
    }
    if(GetKey(olc::ESCAPE).bPressed)
    {
        mainMenuActive = !mainMenuActive;
        editMenuActive = false;
        addMenuActive = false;
        resetOffset();
    }
    if(GetKey(olc::Q).bPressed)
    {
        editMenuActive = !editMenuActive;
        mainMenuActive = false;
        addMenuActive = false;
        resetOffset();
    }
    if(GetKey(olc::W).bHeld)
        menuOffsetY--;
    if(GetKey(olc::S).bHeld)
        menuOffsetY++;
    if(GetKey(olc::A).bHeld)
        menuOffsetX--;
    if(GetKey(olc::D).bHeld)
        menuOffsetX++;

		return true;
	}
};

int main() 
{
    /*
    Cable cable;
    cable.changeValue();
    Switch circuit_switch = cable.getSwitch();
    circuit_switch.activate();
    circuit_switch.deactivate();
    circuit_switch.isActive();
    circuit_switch.isOpen();
    circuit_switch.openSwitch();
    circuit_switch.closeSwitch();
    Circuit circuit;
    circuit.run();


    Transistor transistor;
    transistor.getThreshold();
    transistor.getThresholdVoltage();
*/

    Sim sim;
    if(sim.Construct(1280, 720, 1, 1))
        sim.Start();
}
