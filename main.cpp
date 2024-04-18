#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

enum ElementType 
{
    ELEM_CABLE,
    ELEM_NODE,
    ELEM_RESISTOR,
    ELEM_TRANSISTOR,
    ELEM_SOURCE,
    ELEM_BATTERY
}; 

class CircuitElement
{
protected:
    std::pair<uint32_t, uint32_t> position;

    int32_t voltage;
    int32_t power;
    uint32_t temperature; // useless for now

    CircuitElement* in; // prev element
    CircuitElement* out; // next element
public:
    CircuitElement()
    {
        position = std::make_pair(0, 0);
        this->voltage = 0;
        this->power = 0;
        this->in = NULL;
        this->out = NULL;
        this->temperature = 273;

    }
    
    explicit CircuitElement(const uint32_t posX, const uint32_t posY, const int32_t voltage, const int32_t power = 0, \
            CircuitElement* in = NULL, CircuitElement* out = NULL, \
            const uint32_t temperature = 273)
    {
        this->position = std::make_pair(posX, posY);

        this->voltage = voltage;
        this->power = power;
        
        this->in = in;
        this->out = out;
        
        this->temperature = temperature;
    }

    CircuitElement(const CircuitElement& element)
    {
        this->position= element.position;
        
        this->voltage = element.voltage;
        this->power = element.power;
        
        this->in = element.in;
        this->out = element.out;
        
        this->temperature = element.temperature;
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
                
        this->in = element.in;
        this->out = element.out;

        this->temperature = element.temperature;

        return *this;
    }

    virtual void changeValue(int32_t value) = 0;
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
   
    explicit CableNode(const uint32_t posX, const uint32_t posY, const std::vector<CircuitElement*>& in , const std::vector<CircuitElement*>& out)
    {
        this->position = std::make_pair(posX, posY);
        this->inputs = in; 
        this->outputs = out;
    }
    
    CableNode(const CableNode& node) : CircuitElement(node)
    {
        this->position = node.position;
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
        this->position = element.getPosition();
        this->inputs = element.getIn();
        this->outputs = element.getOut();

        return *this;
    }

    void changeValue(int32_t value)
    {
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

    explicit Transistor(const uint32_t posX, const uint32_t posY, const int32_t voltage = 0, const int32_t power = 0, \
            CircuitElement* in = NULL, CircuitElement* out = NULL,\ 
            const int32_t thresholdVoltage = 0, const uint32_t temperature = 273)
    {
        this->position = std::make_pair(posX, posY);

        this->voltage = voltage;
        this->power = power;
        
        this->in = in;
        this->out = out;
        
        this->threshold = threshold;
        this->thresholdVoltage = thresholdVoltage;

        this->temperature = temperature;
    }

    Transistor(const Transistor& transistor) : CircuitElement(transistor)
    {
        this->position = transistor.position;

        this->voltage = transistor.voltage;
        this->power = transistor.power;
        
        this->in = transistor.in;
        this->out = transistor.out;
        
        this->threshold = transistor.threshold;
        this->thresholdVoltage = transistor.thresholdVoltage;

        this->temperature = transistor.temperature;
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
        this->position = transistor.getPosition();

        this->voltage = transistor.getVoltage();
        this->power = transistor.getPower();

        this->in = transistor.getIn();
        this->out = transistor.getOut();

        this->threshold = transistor.getThreshold();
        this->temperature = transistor.getTemperature();
       
        return *this;
    }

    void changeValue(int32_t value)
    {
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

    explicit Resistor(const uint32_t posX, const uint32_t posY,\ 
            const int32_t voltage = 0, const int32_t power = 0, CircuitElement* in = NULL, CircuitElement* out = NULL, \
            const int32_t resistance = 0, const int32_t powerDissipation = 0, const int32_t tolerance = 0, const uint32_t temperature = 273)
    {
        this->position = std::make_pair(posX, posY);

        this->voltage = voltage;
        this->power = power;

        this->in = in;
        this->out = out;

        this->resistance = resistance;
        this->powerDissipation = powerDissipation;
        this->tolerance = tolerance;

        this->temperature = temperature;
       
    }

    Resistor(const Resistor& r) : CircuitElement(r)
    {
        this->position = r.position;

        this->voltage = r.voltage;
        this->power = r.power;
        
        this->in = r.in;
        this->out = r.out;
        
        this->resistance = r.resistance;
        this->powerDissipation = r.powerDissipation;
        this->tolerance = r.tolerance;

        this->temperature = r.temperature;
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
        this->position = r.getPosition();

        this->voltage = r.getVoltage();
        this->power = r.getPower();
        
        this->in = r.getIn();
        this->out = r.getOut();
        
        this->resistance = r.getResistance();
        this->powerDissipation = r.getPowerDissipation();
        this->tolerance = r.getTolerance();

        this->temperature = r.getTemperature();
                 

        return *this;
    }

    void changeValue(int32_t value)
    {
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
    std::pair<uint32_t, uint32_t> end;
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

    explicit Cable(const uint32_t posX, const uint32_t posY, const std::pair<uint32_t, uint32_t> end = std::make_pair(0, 0), \
            const int32_t voltage = 0, const int32_t power = 0, \
            CircuitElement* in = NULL,CircuitElement* out = NULL, \
            const int32_t resistance = 0, const bool reverse = false, const int32_t length = 0, const uint32_t temperature = 273)
    {
        this->position = std::make_pair(posX, posY);
        this->end = end;

        this->voltage = voltage;
        this->power = power;

        this->in = in;
        this->out = out;

        this->resistance = resistance;
        this->reverse = reverse;

        this->temperature = temperature;
        this->length = length;
    }

    Cable(const Cable& c) : CircuitElement(c)
    {
        this->position = c.position;
        this->end = c.end;
    
        this->voltage = c.voltage;
        this->power = c.power;

        this->in = c.in;
        this->out = c.out;

        this->resistance = c.resistance;
        this->reverse = c.reverse;
                
        this->temperature = c.temperature;
        this->length = c.length;
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
       
        this->voltage = cable.getVoltage();
        this->power = cable.getPower();
        
        this->in = cable.getIn();
        this->out = cable.getOut();

        this->reverse = cable.getFlowDirection();
        this->resistance = cable.getResistance();
                
        this->temperature = cable.getTemperature();
        this->length = cable.getLength();

        return *this;
    }

    void changeValue(int32_t value)
    {
    }
};

class Source : public CircuitElement
{
private:
public:
    Source()
    {
    }
    
    Source(uint32_t posX, uint32_t posY, int32_t voltage = 0, int32_t power = 0, \
            CircuitElement* in = NULL, CircuitElement* out = NULL,\
            uint32_t temperature = 273)
    {
        this->position = std::make_pair(posX, posY);
        
        this->voltage = voltage;
        this->power = power;
        
        this->in = in;
        this->out = out;
        
        this->temperature = temperature;
    }

    Source(const Source& s) : CircuitElement(s)
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
        this->position = s.getPosition();

        this->power = s.getPower();
        this->voltage = s.getVoltage();
        
        this->in = s.getIn();
        this->out = s.getOut();

        this->temperature = s.getTemperature();

        return *this;
    }

    void changeValue(int32_t value)
    {
    }
};


class Battery : public CircuitElement
{
    private:
        uint32_t capacity;
    public:
    Battery()
    {
        this->capacity = 0;
    }

    explicit Battery(const uint32_t posX, const uint32_t posY, const int32_t voltage, const int32_t power = 0, \
        CircuitElement* in = NULL, CircuitElement* out = NULL, \
        const uint32_t capacity = 0, const uint32_t temperature = 273)
    {
        this->position = std::make_pair(posX, posY);

        this->voltage = voltage;
        this->power = power;
        
        this->in = in;
        this->out = out;
        
        this->capacity = capacity;
        this->temperature = temperature;
    }

    Battery(const Battery& b) : CircuitElement(b)
    {
        this->position = b.position;
        
        this->voltage = b.voltage;
        this->power = b.power;

        this->out = b.out;
        this->in = b.in;
        
        this->capacity = b.capacity;
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
        
        this->voltage = b.getVoltage();
        this->power = b.getPower();
        
        this->out = b.getOut();
        this->in = b.getIn();
        
        this->capacity = b.getCapacity();
        this->temperature = b.getTemperature();

        return *this;
    }

    void changeValue(int32_t value)
    {
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

    void pressEntry(const int32_t posX, const int posY, std::string key, std::string action)
    {
        DrawString(posX, posY, "Press ");
        DrawString(posX + 45, posY, key, olc::GREEN);
        DrawString(posX + 50, posY, " to ");
        DrawString(posX + 80, posY, action);
    }

    void addElement(ElementType type)
    {
        CircuitElement* element;
        switch(type)
        {
            case ELEM_CABLE:
                element = new Cable();
                break;
            case ELEM_NODE:
                element = new CableNode();
                break;
            case ELEM_RESISTOR:
                element = new Resistor();
                break;
            case ELEM_TRANSISTOR:
                element = new Transistor();
                break;
            case ELEM_SOURCE:
                element = new Source();
                break;
            case ELEM_BATTERY:
                element = new Battery();
                break;
            default:
                std::cout<<"Element invalid.\n";
                return;
        }
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

        if(GetKey(olc::K1).bPressed)
        {
            addElement(ELEM_CABLE);
        }
        else if(GetKey(olc::K2).bPressed)
        {
            addElement(ELEM_NODE);
        }
        else if(GetKey(olc::K3).bPressed)
        {
            addElement(ELEM_RESISTOR);
        }
        else if(GetKey(olc::K4).bPressed)
        {
            addElement(ELEM_TRANSISTOR);
        }
        else if(GetKey(olc::K5).bPressed)
        {
            addElement(ELEM_SOURCE);
        }
        else if(GetKey(olc::K6).bPressed)
        {
            addElement(ELEM_BATTERY);
        }
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
