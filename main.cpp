#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

enum ElementType 
{
    ELEM_UNASSIGNED,
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
    olc::vi2d position;
    int32_t voltage;
    int32_t power;
    uint32_t temperature; // useless for now

    CircuitElement* in; // prev element
    CircuitElement* out; // next element
public:
    CircuitElement()
    {
        position = {0, 0};
        this->voltage = 0;
        this->power = 0;
        this->in = NULL;
        this->out = NULL;
        this->temperature = 273;
    }
    
    explicit CircuitElement(const int32_t posX, const int32_t posY, const int32_t voltage, const int32_t power = 0, \
            CircuitElement* in = NULL, CircuitElement* out = NULL, \
            const uint32_t temperature = 273)
    {
        this->position = {posX, posY};

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
    
    olc::vi2d getPosition() { return this->position; };
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
    virtual void drawYourself(olc::PixelGameEngine *pge, const float scale = 10.0f) = 0;
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
   
    explicit CableNode(const int32_t posX, const int32_t posY)
    {
        this->position = {posX, posY};
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

    virtual void drawYourself(olc::PixelGameEngine *pge, const float scale = 10.0f)
    {
        pge->DrawCircle(this->position.x, this->position.y, scale);
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

    explicit Transistor(const int32_t posX, const int32_t posY, const int32_t voltage = 0, const int32_t power = 0, \
            CircuitElement* in = NULL, CircuitElement* out = NULL,\ 
            const int32_t thresholdVoltage = 0, const uint32_t temperature = 273)
    {
        this->position = {posX, posY};

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

    virtual void drawYourself(olc::PixelGameEngine *pge, const float scale = 10.0f)
    {
        pge->DrawCircle(this->position.x, this->position.y, 10);
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

    explicit Resistor(const int32_t posX, const int32_t posY,\ 
            const int32_t voltage = 0, const int32_t power = 0, CircuitElement* in = NULL, CircuitElement* out = NULL, \
            const int32_t resistance = 0, const int32_t powerDissipation = 0, const int32_t tolerance = 0, const uint32_t temperature = 273)
    {
        this->position = {posX, posY};

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
    virtual void drawYourself(olc::PixelGameEngine *pge, const float scale = 10.0f)
    {
        pge->DrawCircle(this->position.x, this->position.y, scale);
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
    std::string drawing[8];
        this->open = false;
    }
};

class Cable : public CircuitElement
{
private:
    olc::vi2d end;
    int32_t resistance;
    bool reverse;
    int32_t length;
    Switch circuitSwitch;
public:
    Cable()
    {
        this->end = {0,0};
        this->length = 0;
        this->resistance = 0;
        this->reverse = false;
    }

    explicit Cable(const int32_t posX, const int32_t posY, const olc::vi2d end = {0, 0}, \
            const int32_t voltage = 0, const int32_t power = 0, \
            CircuitElement* in = NULL,CircuitElement* out = NULL, \
            const int32_t resistance = 0, const bool reverse = false, const int32_t length = 0, const uint32_t temperature = 273)
    {
        this->position = {posX, posY};
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

    olc::vi2d getEnd() { return this->end; };
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
    virtual void drawYourself(olc::PixelGameEngine *pge, const float scale)
    {
        pge->DrawCircle(this->position.x, this->position.y, scale);
    }
};

class Source : public CircuitElement
{
private:
public:
    Source()
    {
    }
    
    Source(int32_t posX, int32_t posY, int32_t voltage = 0, int32_t power = 0, \
            CircuitElement* in = NULL, CircuitElement* out = NULL,\
            uint32_t temperature = 273)
    {
        this->position = {posX, posY};
        
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
    virtual void drawYourself(olc::PixelGameEngine *pge, const float scale)
    {
        pge->DrawCircle(this->position.x, this->position.y, scale);
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

    explicit Battery(const int32_t posX, const int32_t posY, const int32_t voltage = 0, const int32_t power = 0, \
        CircuitElement* in = NULL, CircuitElement* out = NULL, \
        const uint32_t capacity = 0, const uint32_t temperature = 273)
    {
        this->position = {posX, posY};

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
    virtual void drawYourself(olc::PixelGameEngine *pge, const float scale = 10.0f)
    {
        pge->DrawCircle(this->position.x, this->position.y, scale);
    }
};

class Circuit
{
private:
    int32_t voltageIn;
    int32_t voltageOut;
    std::vector<CircuitElement*> elements;
public:
    void addElementToCircuit(CircuitElement* element)
    {
        elements.push_back(element);
    }

    Circuit()
    {
        this->voltageIn = 0;
        this->voltageOut = 0;        
    }

    Circuit(const Circuit& circuit)
    {
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

    int32_t getVoltageIn() { return this->voltageIn; };
    int32_t getVoltageOut() { return this->voltageOut; };
    std::vector<CircuitElement*>& getElements() { return this->elements; };
    Circuit& operator=(Circuit& circuit)
    {
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
    bool mainMenuActive = false;
    bool editMenuActive = false;
    bool removeMenuActive = false;
    bool addMenuActive = false;
    bool modifyMenuActive = false;
    bool addElement = false;

    olc::vi2d menuOffset = {0, 0};
    olc::vf2d worldOffset = {0.0f, 0.0f};
    
    float scale = 10.0f;

    olc::vf2d mousePos;
    olc::vf2d startPan;

    CircuitElement* tempElement = NULL;
    ElementType tempType = ELEM_UNASSIGNED;
    olc::vi2d tempPos;
        
    Circuit circuit;

    void resetOffset()
    {
        menuOffset = {0,0};
    }

    void pressEntry(olc::vi2d pos, std::string key, std::string action)
    {
        DrawString(pos.x     , pos.y, "Press ");
        DrawString(pos.x + 45, pos.y, key, olc::GREEN);
        DrawString(pos.x + 50, pos.y, " to ");
        DrawString(pos.x + 80, pos.y, action);
    }
    void pressEntry(olc::vi2d pos, std::string key1, std::string key2, std::string action)
    {
        DrawString(pos.x   , pos.y, "Press ");
        DrawString(pos.x + 45, pos.y, key1, olc::GREEN);
        DrawString(pos.x + 85, pos.y, ", ");
        DrawString(pos.x + 95, pos.y, key2, olc::GREEN);
        DrawString(pos.x + 100, pos.y, " to ");
        DrawString(pos.x + 130, pos.y, action);
    }
   
    void drawElement(CircuitElement* element)
    {
        element->drawYourself(this, scale);
    }

    void drawCircuit()
    {
        for(int i = 0; i < circuit.getElements().size(); i++)
        {
            drawElement(circuit.getElements()[i]); 
        }
    }

    void resetTempCoord()
    {
        tempPos.x = GetScreenSize().x / 2;
        tempPos.y = GetScreenSize().y / 2;
    }

    void WorldToScreen(olc::vf2d& v, int& screenX, int& screenY)
    {
        screenX = (int)((v.x - worldOffset.x) * scale);
        screenY = (int)((v.y - worldOffset.y) * scale);
    }

    void ScreenToWorld(int screenX, int screenY, olc::vf2d& v)
    {
        v.x = (float)(screenX) / scale + worldOffset.x;
        v.y = (float)(screenY) / scale + worldOffset.y;
    }
public:
	Sim()
	{
		sAppName = "Circuit Simulator";
	}

public:
	bool OnUserCreate() override
	{
    worldOffset.x = (float)(-GetScreenSize().x / 2) / scale;
    worldOffset.y = (float)(-GetScreenSize().y / 2) / scale;

    mainMenuActive = true;
    resetTempCoord();

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
    mousePos = {(float)GetMouseX(), (float)GetMouseY() };

    Clear(olc::BLACK);
    drawCircuit();

    if(addElement)
    {
        editMenuActive = false;
        mainMenuActive = false;
        addMenuActive = false;
        removeMenuActive = false;
        modifyMenuActive = false;

        if(GetKey(olc::ENTER).bPressed) // add element to circuit
        {
            CircuitElement* element;
            switch(tempType)
            {
                case ELEM_CABLE:
                    element = new Cable(tempPos.x, tempPos.y);
                    break;
                case ELEM_NODE:
                    element = new CableNode(tempPos.x, tempPos.y);
                    break;
                case ELEM_RESISTOR:
                    element = new Resistor(tempPos.x, tempPos.y);
                    break;
                case ELEM_TRANSISTOR:
                    element = new Transistor(tempPos.x, tempPos.y);
                    break;
                case ELEM_SOURCE:
                    element = new Source(tempPos.x, tempPos.y);
                    break;
                case ELEM_BATTERY:
                    element = new Battery(tempPos.x, tempPos.y);
                    break;
                default:
                    return false;
            }
            circuit.addElementToCircuit(element);
        }
        DrawCircle(tempPos.x, tempPos.y, scale);
        if(GetKey(olc::W).bHeld)
        {
            tempPos.y -= (int)(std::sqrt(scale));
        }
        if(GetKey(olc::S).bHeld)
        {
            tempPos.y += (int)(std::sqrt(scale));
        }
        if(GetKey(olc::A).bHeld)
        {
            tempPos.x -= (int)(std::sqrt(scale));
        }
        if(GetKey(olc::D).bHeld)
        {
            tempPos.x += (int)(std::sqrt(scale));
        }
    }

    if(mainMenuActive)
    {
        FillRect(50 + menuOffset.x, 50 + menuOffset.y, 500, 200, olc::BLACK);
        DrawRect(50 + menuOffset.x, 50 + menuOffset.y, 500, 200, olc::WHITE);
        DrawString(70 + menuOffset.x, 70 + menuOffset.y, "CCircuit Designer - Commands", olc::WHITE, 2);
        DrawString(70 + menuOffset.x, 90 + menuOffset.y, "Press");
        DrawString(115 + menuOffset.x, 90 + menuOffset.y, "ESC", olc::GREEN);
        DrawString(145 + menuOffset.x, 90 + menuOffset.y, "to open/close this window");
        pressEntry({70 + menuOffset.x, 110 + menuOffset.y}, "Q", "open edit menu");
        pressEntry({70 + menuOffset.x, 130 + menuOffset.y}, "R", "start/stop simulation");
        pressEntry({70 + menuOffset.x, 150 + menuOffset.y}, "SHIFT", "W", "move window up");
        pressEntry({70 + menuOffset.x, 160 + menuOffset.y}, "SHIFT", "S", "move window down");
        pressEntry({70 + menuOffset.x, 170 + menuOffset.y}, "SHIFT", "A", "move window left");
        pressEntry({70 + menuOffset.x, 180 + menuOffset.y}, "SHIFT", "D", "move window right");
    }
    if(addMenuActive)
    {
        FillRect(50 + menuOffset.x, 50 + menuOffset.y, 500, 200, olc::BLACK);
        DrawRect(50 + menuOffset.x, 50 + menuOffset.y, 500, 200, olc::WHITE);
        DrawString(70 + menuOffset.x, 70 + menuOffset.y, "Edit circuit", olc::WHITE, 2);
        pressEntry({70 + menuOffset.x, 90 + menuOffset.y}, "Q", "go back to edit menu");
        pressEntry({70 + menuOffset.x, 110 + menuOffset.y}, "1", "add cable");
        pressEntry({70 + menuOffset.x, 130 + menuOffset.y}, "2", "add cable node");
        pressEntry({70 + menuOffset.x, 150 + menuOffset.y}, "3", "add resistor");
        pressEntry({70 + menuOffset.x, 170 + menuOffset.y}, "4", "add transistor");
        pressEntry({70 + menuOffset.x, 190 + menuOffset.y}, "5", "add source");
        pressEntry({70 + menuOffset.x, 210 + menuOffset.y}, "6", "add battery");

        if(GetKey(olc::K1).bPressed)
        {
            addElement = true;
            tempType = ELEM_CABLE;
            resetTempCoord();
        }
        else if(GetKey(olc::K2).bPressed)
        {
            addElement = true;
            tempType = ELEM_NODE;
            resetTempCoord();
        }
        else if(GetKey(olc::K3).bPressed)
        {
            addElement = true;
            tempType = ELEM_RESISTOR;
            resetTempCoord();
        }
        else if(GetKey(olc::K4).bPressed)
        {
            addElement = true;
            tempType = ELEM_TRANSISTOR;
            resetTempCoord();
        }
        else if(GetKey(olc::K5).bPressed)
        {
            addElement = true;
            tempType = ELEM_SOURCE;
            resetTempCoord();
        }
        else if(GetKey(olc::K6).bPressed)
        {
            addElement = true;
            tempType = ELEM_BATTERY;
            resetTempCoord();
        }
    }
    
    if(editMenuActive)
    {
        FillRect(50 + menuOffset.x, 50 + menuOffset.y, 500, 200, olc::BLACK);
        DrawRect(50 + menuOffset.x, 50 + menuOffset.y, 500, 200, olc::WHITE);
        DrawString(70 + menuOffset.x, 70 + menuOffset.y, "Edit circuit", olc::WHITE, 2);
        pressEntry({70 + menuOffset.x, 90 + menuOffset.y}, "Q", "open/close this window");
        pressEntry({70 + menuOffset.x, 110 + menuOffset.y}, "1","add elements to circuit");
        pressEntry({70 + menuOffset.x, 130 + menuOffset.y}, "2","remove elements from circuit");
        pressEntry({70 + menuOffset.x, 150 + menuOffset.y}, "3","edit elements in circuit");
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
        if(!addElement)
            mainMenuActive = !mainMenuActive;
        editMenuActive = false;
        addMenuActive = false;
        addElement = false;
        resetOffset(); 
    }
    if(GetKey(olc::Q).bPressed)
    {
        editMenuActive = !editMenuActive;
        mainMenuActive = false;
        addMenuActive = false;
        resetOffset();
    }

    if(GetMouse(2).bPressed)
    {
        startPan = mousePos;
    }

    if(GetMouse(2).bHeld)
    {
        worldOffset -= (mousePos - startPan) / scale; 
        startPan = mousePos;
    }

    if(GetKey(olc::SHIFT).bHeld && GetKey(olc::W).bHeld)
        menuOffset.y-=(int)(std::sqrt(scale));
    if(GetKey(olc::SHIFT).bHeld && GetKey(olc::S).bHeld)
        menuOffset.y+=(int)(std::sqrt(scale));
    if(GetKey(olc::SHIFT).bHeld && GetKey(olc::A).bHeld)
        menuOffset.x-=(int)(std::sqrt(scale));
    if(GetKey(olc::SHIFT).bHeld && GetKey(olc::D).bHeld)
        menuOffset.x+=(int)(std::sqrt(scale));

    olc::vf2d mousePosBZoom;
    ScreenToWorld((int)mousePos.x, (int)mousePos.y, mousePosBZoom);

    if(GetKey(olc::EQUALS).bPressed || GetMouseWheel() > 0)
        scale *= 1.1f;
    if(GetKey(olc::MINUS).bPressed || GetMouseWheel() < 0)
        scale *= 0.9f;
	  
    olc::vf2d mousePosAZoom;
    ScreenToWorld((int)mousePos.x, (int)mousePos.y, mousePosAZoom);
    worldOffset += (mousePosBZoom - mousePosAZoom);

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
