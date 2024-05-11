#include "circuit_elements.hpp"

        CircuitElement::CircuitElement()
        {
            this->voltage = 0;
            this->power = 0;
            this->in = NULL;
            this->out = NULL;
            this->temperature = 273;
        }

        CircuitElement::CircuitElement(const int32_t posX, const int32_t posY, const int32_t voltage, const int32_t power, \
                CircuitElement* in, CircuitElement* out, \
                const uint32_t temperature)
        {
            this->position = {posX, posY};

            this->voltage = voltage;
            this->power = power;

            this->in = in;
            this->out = out;

            this->temperature = temperature;
        }

        CircuitElement::CircuitElement(const CircuitElement& element)
        {
            this->position= element.position;

            this->voltage = element.voltage;
            this->power = element.power;

            this->in = element.in;
            this->out = element.out;

            this->temperature = element.temperature;
        }

        CircuitElement::~CircuitElement()
        {
        }

        olc::vi2d CircuitElement::getPosition() const { return this->position; }
        int32_t CircuitElement::getVoltage() const { return this->voltage; }
        int32_t CircuitElement::getPower() const { return this->power; }
        int32_t CircuitElement::getTemperature() const { return this->temperature; }
        CircuitElement* CircuitElement::getIn() const { return this->in; }
        CircuitElement* CircuitElement::getOut() const { return this->out; }

         std::ostream& operator<<(std::ostream& os, const CircuitElement& el)
        {
            os << "Voltage: "<<el.getVoltage() <<"    Power: " <<el.getPower() <<"    Temperature: "<<el.getTemperature()<<"\n";
            return os;        
        }

        CircuitElement& CircuitElement::operator=(const CircuitElement& element)
        {
            this->position = element.position;

            this->voltage = element.voltage;
            this->power = element.power;

            this->in = element.in;
            this->out = element.out;

            this->temperature = element.temperature;

            return *this;
        }

        void CircuitElement::WorldToScreen(const olc::vf2d& v, const olc::vf2d& worldOffset, const float worldScale, int& screenX, int& screenY)
        {
            screenX = (int)((v.x - worldOffset.x) * worldScale);
            screenY = (int)((v.y - worldOffset.y) * worldScale);
        }

        CableNode::CableNode()   
        {
        }

        CableNode::CableNode(const int32_t posX, const int32_t posY)
        {
            this->position = {posX, posY};
        }

        CableNode::CableNode(const CableNode& node) : CircuitElement(node)
        {
            this->position = node.position;
            this->inputs = node.inputs;
            this->outputs = node.outputs;
        }

        CableNode::~CableNode()
        {

        }

        std::ostream& operator<<(std::ostream& os, const CableNode& el)
        {
            os<<(el.getInputs()).size()<<" "<<(el.getOutputs()).size();
            return os;        
        }

        std::vector<CircuitElement*> CableNode::getInputs() const { return this->inputs; }
        std::vector<CircuitElement*> CableNode::getOutputs() const { return this->outputs; }

        CableNode& CableNode::operator=(const CableNode& element)
        {
            this->position = element.getPosition();
            this->inputs = element.getInputs();
            this->outputs = element.getOutputs();

            return *this;
        }

        void CableNode::changeValue(int32_t& value) 
        {
            value = 0;
        }

        void CableNode::drawYourself(olc::PixelGameEngine *pge, const olc::vf2d worldOffset, const float worldScale) 
        {
            int nx, ny;
            WorldToScreen(this->position, worldOffset, worldScale, nx, ny);
            pge->DrawCircle(nx, ny, worldScale);
        }
 
        Transistor::Transistor() 
        {
            this->threshold = 0;
            this->thresholdVoltage = 0;
        }

        Transistor::Transistor(const int32_t posX, const int32_t posY, const int32_t voltage, const int32_t power, \
                CircuitElement* in, CircuitElement* out,\
                const int32_t threshold, const int32_t thresholdVoltage, const uint32_t temperature)
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

        Transistor::Transistor(const Transistor& transistor) : CircuitElement(transistor) 
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

        Transistor::~Transistor()
        {
        }

        std::ostream& operator<<(std::ostream& os, const Transistor& el)
        {
            os<<"Threshold value: "<<el.getThreshold()<<"    Threshold voltage: "<<el.getThresholdVoltage()<<"\n";
            os << "Voltage: "<<el.getVoltage() <<"    Power: " <<el.getPower() <<"    Temperature: "<<el.getTemperature()<<"\n";

            return os;
        }

        int32_t Transistor::getThreshold() const { return this->threshold; }
        int32_t Transistor::getThresholdVoltage() const { return this->thresholdVoltage; }

        Transistor& Transistor::operator=(const Transistor& transistor)
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

        void Transistor::changeValue(int32_t& value) 
        {
            value = 10;
        }

        void Transistor::drawYourself(olc::PixelGameEngine *pge, const olc::vf2d worldOffset, const float worldScale) 
        {
            int nx, ny;
            WorldToScreen(this->position, worldOffset, worldScale, nx, ny);
            pge->DrawCircle(nx, ny, worldScale);
        }

        Resistor::Resistor()
        {
            this->resistance = 0;
            this->powerDissipation = 0;
            this->tolerance = 0;
        }

        Resistor::Resistor(const int32_t posX, const int32_t posY,\
                const int32_t voltage, const int32_t power, CircuitElement* in, CircuitElement* out, \
                const int32_t resistance, const int32_t powerDissipation, const int32_t tolerance, const uint32_t temperature)
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

        Resistor::Resistor(const Resistor& r) : CircuitElement(r)
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

        Resistor::~Resistor()
        {
        }

        std::ostream& operator<<(std::ostream& os, const Resistor& el)
        {
            os<<"Resistance: "<<el.getResistance()<<"    Power that can be safely dissipated: "<<el.getPowerDissipation()<<"   Tolerance: "<<el.getTolerance()<<"\n";
            os << "Voltage: "<<el.getVoltage() <<"    Power: " <<el.getPower() <<"    Temperature: "<<el.getTemperature()<<"\n";

            return os; 
        }

        int32_t Resistor::getResistance() const { return this->resistance; }
        int32_t Resistor::getPowerDissipation() const { return this->powerDissipation; }
        int32_t Resistor::getTolerance() const { return this->tolerance; }

        Resistor& Resistor::operator=(const Resistor& r)
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

        void Resistor::changeValue(int32_t& value) 
        {
            value = 10;
        }
        void Resistor::drawYourself(olc::PixelGameEngine *pge, const olc::vf2d worldOffset, const float worldScale) 
        {
            int nx, ny;
            WorldToScreen(this->position, worldOffset, worldScale, nx, ny);
            pge->DrawCircle(nx, ny, worldScale);
        }

        Switch::Switch()
        {
            open = false;
            active = false; // determines if the switch is drawn or not
        }

        Switch::Switch(const Switch& s) 
        {
            this->open = s.open;
            this->active = s.active;
        }

        Switch::~Switch()
        {
        }

        bool Switch::isOpen()
        {
            return open;
        }

        bool Switch::isActive()
        {
            return active;
        }

        void Switch::activate()
        {
            this->active = true;
        }

        void Switch::deactivate()
        {
            this->active = false;
        }

        void Switch::openSwitch()
        {
            this->open = true;
        }

        void Switch::closeSwitch()
        {
            this->open = false;
        }

        Cable::Cable()
        {
            this->end = {0,0};
            this->length = 0;
            this->resistance = 0;
            this->reverse = false;
        }

        Cable::Cable(const int32_t posX, const int32_t posY, const olc::vi2d end, \
                const int32_t voltage, const int32_t power, \
                CircuitElement* in, CircuitElement* out, \
                const int32_t resistance, const bool reverse, const int32_t length, const uint32_t temperature)
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

        Cable::Cable(const Cable& c) : CircuitElement(c) 
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

        Cable::~Cable()
        {
        }

        std::ostream& operator<<(std::ostream& os, const Cable& el)
        {
            os <<"Resistance: "<<el.getResistance()<<"   Voltage: "<<el.getVoltage()<<"    Power: " <<el.getPower()<<"    Temperature: "<<el.getTemperature()<<"\n";

            return os;
        }

        olc::vi2d Cable::getEnd() const { return this->end; }
        int32_t Cable::getResistance() const { return this->resistance; }
        Switch Cable::getSwitch() const { return this->circuitSwitch; }
        bool Cable::getFlowDirection() const { return this->reverse; }
        int32_t Cable::getLength() const { return this->length; }
        Cable& Cable::operator=(const Cable& cable)
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

        void Cable::changeValue(int32_t& value) 
        {
            value = 10;
        }
        void Cable::drawYourself(olc::PixelGameEngine *pge, const olc::vf2d worldOffset, const float worldScale) 
        {
            int nx, ny;
            WorldToScreen(this->position, worldOffset, worldScale, nx, ny);
            pge->DrawCircle(nx, ny, worldScale);
        }

        Source::Source()
        {
        }

        Source::Source(int32_t posX, int32_t posY, int32_t voltage, int32_t power, \
                CircuitElement* in, CircuitElement* out,\
                uint32_t temperature)
        {
            this->position = {posX, posY};

            this->voltage = voltage;
            this->power = power;

            this->in = in;
            this->out = out;

            this->temperature = temperature;
        }

        Source::Source(const Source& s) : CircuitElement(s)
        {
        }

        Source::~Source()
        {
        }

        std::ostream& operator<<(std::ostream& os, const Source& el)
        {
            os <<"Voltage: "<<el.getVoltage() <<"    Power: " <<el.getPower() <<"    Temperature: "<<el.getTemperature()<<"\n";

            return os;
        }

        Source& Source::operator=(const Source& s) 
        {
            this->position = s.getPosition();

            this->power = s.getPower();
            this->voltage = s.getVoltage();

            this->in = s.getIn();
            this->out = s.getOut();

            this->temperature = s.getTemperature();

            return *this;
        }

        void Source::changeValue(int32_t& value) 
        {
            value = 10;
        }
        void Source::drawYourself(olc::PixelGameEngine *pge, const olc::vf2d worldOffset, const float worldScale) 
        {
            int nx, ny;
            WorldToScreen(this->position, worldOffset, worldScale, nx, ny);
            pge->DrawCircle(nx, ny, worldScale);
        }

        Battery::Battery()
        {
            this->capacity = 0;
        }

        Battery::Battery(const int32_t posX, const int32_t posY, const int32_t voltage, const int32_t power, \
                CircuitElement* in, CircuitElement* out, \
                const uint32_t capacity, const uint32_t temperature)
        {
            this->position = {posX, posY};

            this->voltage = voltage;
            this->power = power;

            this->in = in;
            this->out = out;

            this->capacity = capacity;
            this->temperature = temperature;
        }

        Battery::Battery(const Battery& b) : CircuitElement(b)
        {
            this->position = b.position;

            this->voltage = b.voltage;
            this->power = b.power;

            this->out = b.out;
            this->in = b.in;

            this->capacity = b.capacity;
            this->temperature = b.temperature;
        }

        Battery::~Battery()
        {
        }

        std::ostream& operator<<(std::ostream& os, const Battery& el)
        {
            os<<"Capacity: "<<el.getCapacity()<<" Voltage: "<<el.getVoltage()<<"\n";
            return os;
        }
        int32_t Battery::getCapacity() const { return this->capacity; };

        Battery& Battery::operator=(const Battery& b)
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

        void Battery::changeValue(int32_t& value) 
        {
            value = 10;
        }
        void Battery::drawYourself(olc::PixelGameEngine *pge, const olc::vf2d worldOffset, const float worldScale) 
        {
            int nx, ny;
            WorldToScreen(this->position, worldOffset, worldScale, nx, ny);
            pge->DrawCircle(nx, ny, worldScale);
        }
