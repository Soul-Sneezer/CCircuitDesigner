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
            this->position = element.position;
            this->voltage = element.voltage;
            this->power = element.power;
            this->in = element.in;
            this->out = element.out;
            this->temperature = element.temperature;
        }

        CircuitElement::CircuitElement(CircuitElement&& element)
        {
        }

        CircuitElement& CircuitElement::operator=(const CircuitElement& element)
        {
            this->position = element.getPosition();
            this->voltage = element.getVoltage();
            this->power = element.getPower();
            this->in = element.getIn();
            this->out = element.getOut();
            this->temperature = element.getTemperature();
            
            return *this;
        }

        CircuitElement& CircuitElement::operator=(CircuitElement&& element)
        {
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

        std::ostream& CircuitElement::operator<<(std::ostream& os) 
        {
            os << "Voltage: "<<this->getVoltage() <<"    Power: " <<this->getPower() <<"    Temperature: "<<this->getTemperature()<<"\n";
            return os;        
        }

        void CircuitElement::WorldToScreen(const olc::vf2d& v, int& screenX, int& screenY)
        {
            screenX = (int)((v.x - CircuitElement::getWorldOffset().x) * CircuitElement::getWorldScale());
            screenY = (int)((v.y - CircuitElement::getWorldOffset().y) * CircuitElement::getWorldScale());
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
            this->inputs = node.inputs;
            this->outputs = node.outputs;
        }

        CableNode::CableNode(CableNode&& node)
        {
        }

        CableNode::~CableNode()
        {

        }

        std::ostream& CableNode::operator<<(std::ostream& os) 
        {
            os<<(this->getInputs()).size()<<" "<<(this->getOutputs()).size();
            return os;        
        }

        std::vector<std::shared_ptr<CircuitElement>> CableNode::getInputs() const { return this->inputs; }
        std::vector<std::shared_ptr<CircuitElement>> CableNode::getOutputs() const { return this->outputs; }

        CableNode& CableNode::operator=(const CableNode& element) 
        {
            this->inputs = element.getInputs();
            this->outputs = element.getOutputs();
            
            this->position = element.getPosition();
            this->voltage = element.getVoltage();
            this->power = element.getPower();
            this->in = element.getIn();
            this->out = element.getOut();
            this->temperature = element.getTemperature();

            return *this;
        }
        
        CableNode& CableNode::operator=(CableNode&& element)
        {
        }

        void CableNode::changeValue(int32_t& value) 
        {
            value = 0;
        }

        void CableNode::drawYourself(olc::PixelGameEngine *pge) 
        {
            int nx, ny;
            WorldToScreen(this->position, nx, ny);
            pge->DrawCircle(nx, ny, CircuitElement::getWorldScale());
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
            this->threshold = transistor.threshold;
            this->thresholdVoltage = transistor.thresholdVoltage;
        }

        Transistor::Transistor(Transistor&& transistor)
        {
        }

        Transistor::~Transistor()
        {
        }

        std::ostream& Transistor::operator<<(std::ostream& os)
        {
            os<<"Threshold value: "<<this->getThreshold()<<"    Threshold voltage: "<<this->getThresholdVoltage()<<"\n";
            os << "Voltage: "<<this->getVoltage() <<"    Power: " <<this->getPower() <<"    Temperature: "<<this->getTemperature()<<"\n";

            return os;
        }

        int32_t Transistor::getThreshold() const { return this->threshold; }
        int32_t Transistor::getThresholdVoltage() const { return this->thresholdVoltage; }

        Transistor& Transistor::operator=(const Transistor& transistor) 
        {
            this->threshold = transistor.getThreshold();
            this->thresholdVoltage = transistor.getThresholdVoltage();
            
            this->position = transistor.getPosition();
            this->voltage = transistor.getVoltage();
            this->power = transistor.getPower();
            this->in = transistor.getIn();
            this->out = transistor.getOut();
            this->temperature = transistor.getTemperature();

            return *this;
        }

        Transistor& Transistor::operator=(Transistor&& transistor)
        {   
        }

        void Transistor::changeValue(int32_t& value) 
        {
            value = 10;
        }

        void Transistor::drawYourself(olc::PixelGameEngine *pge) 
        {
            int nx, ny;
            WorldToScreen(this->position, nx, ny);
            pge->DrawCircle(nx, ny, CircuitElement::getWorldScale());
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
            this->resistance = r.resistance;
            this->powerDissipation = r.powerDissipation;
            this->tolerance = r.tolerance;
        }

        Resistor::Resistor(Resistor&& r)
        {
        }

        Resistor::~Resistor()
        {
        }

        std::ostream& Resistor::operator<<(std::ostream& os)
        {
            os<<"Resistance: "<<this->getResistance()<<"    Power that can be safely dissipated: "<<this->getPowerDissipation()<<"   Tolerance: "<<this->getTolerance()<<"\n";
            os << "Voltage: "<<this->getVoltage() <<"    Power: " <<this->getPower() <<"    Temperature: "<<this->getTemperature()<<"\n";

            return os; 
        }

        int32_t Resistor::getResistance() const { return this->resistance; }
        int32_t Resistor::getPowerDissipation() const { return this->powerDissipation; }
        int32_t Resistor::getTolerance() const { return this->tolerance; }

        Resistor& Resistor::operator=(const Resistor& r) 
        {
            this->resistance = r.getResistance();
            this->powerDissipation = r.getPowerDissipation();
            this->tolerance = r.getTolerance();

            this->position = r.getPosition();
            this->voltage = r.getVoltage();
            this->power = r.getPower();
            this->in = r.getIn();
            this->out = r.getOut();
            this->temperature = r.getTemperature();

            return *this;
        }

        Resistor& Resistor::operator=(Resistor&& r)
        {
        }

        void Resistor::changeValue(int32_t& value) 
        {
            value = 10;
        }
        void Resistor::drawYourself(olc::PixelGameEngine *pge) 
        {
            int nx, ny;
            WorldToScreen(this->position, nx, ny);
            pge->DrawCircle(nx, ny, CircuitElement::getWorldScale());
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
            this->end = c.end;
            this->resistance = c.resistance;
            this->reverse = c.reverse;
            this->length = c.length;
        }

        Cable::Cable(Cable&& c)
        {
        }

        Cable::~Cable()
        {
        }

        std::ostream& Cable::operator<<(std::ostream& os)
        {
            os <<"Resistance: "<<this->getResistance()<<"   Voltage: "<<this->getVoltage()<<"    Power: " <<this->getPower()<<"    Temperature: "<<this->getTemperature()<<"\n";

            return os;
        }

        olc::vi2d Cable::getEnd() const { return this->end; }
        int32_t Cable::getResistance() const { return this->resistance; }
        Switch Cable::getSwitch() const { return this->circuitSwitch; }
        bool Cable::getFlowDirection() const { return this->reverse; }
        int32_t Cable::getLength() const { return this->length; }
     
        Cable& Cable::operator=(const Cable& cable) 
        {
            this->end = cable.getEnd();
            this->reverse = cable.getFlowDirection();
            this->resistance = cable.getResistance();
            this->length = cable.getLength();

            this->position = cable.getPosition();
            this->voltage = cable.getVoltage();
            this->power = cable.getPower();
            this->in = cable.getIn();
            this->out = cable.getOut();
            this->temperature = cable.getTemperature();

            return *this;
        }

        Cable& Cable::operator=(Cable&& cable)
        {
        }
        
        void Cable::changeValue(int32_t& value) 
        {
            value = 10;
        }
        void Cable::drawYourself(olc::PixelGameEngine *pge) 
        {
            int nx, ny;
            WorldToScreen(this->position, nx, ny);
            pge->DrawCircle(nx, ny, CircuitElement::getWorldScale());
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

        Source::Source(Source&& s)
        {
        }
        
        Source::~Source()
        {
        }

        std::ostream& Source::operator<<(std::ostream& os)
        {
            os <<"Voltage: "<<this->getVoltage() <<"    Power: " <<this->getPower() <<"    Temperature: "<<this->getTemperature()<<"\n";

            return os;
        }

        Source& Source::operator=(const Source& s) 
        {
            this->position = s.getPosition();
            this->voltage = s.getVoltage();
            this->power = s.getPower();
            this->in = s.getIn();
            this->out = s.getOut();
            this->temperature = s.getTemperature();

            return *this;
        }

        Source& Source::operator=(Source&& s)
        {
        }

        void Source::changeValue(int32_t& value) 
        {
            value = 10;
        }
        void Source::drawYourself(olc::PixelGameEngine *pge) 
        {
            int nx, ny;
            WorldToScreen(this->position, nx, ny);
            pge->DrawCircle(nx, ny, CircuitElement::getWorldScale());
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
            this->capacity = b.capacity;
        }

        Battery::Battery(Battery&& b)
        {
        }
    
        Battery::~Battery()
        {
        }

        std::ostream& Battery::operator<<(std::ostream& os)
        {
            os<<"Capacity: "<<this->getCapacity()<<" Voltage: "<<this->getVoltage()<<"\n";
            return os;
        }
        int32_t Battery::getCapacity() const { return this->capacity; };

        Battery& Battery::operator=(const Battery& b) 
        {
            this->capacity = b.getCapacity();
            
            this->position = b.getPosition();
            this->voltage = b.getVoltage();
            this->power = b.getPower();
            this->in = b.getIn();
            this->out = b.getOut();
            this->temperature = b.getTemperature();

            return *this;
        }

        Battery& Battery::operator=(Battery&& b)
        {
        }

        void Battery::changeValue(int32_t& value) 
        {
            value = 10;
        }
        void Battery::drawYourself(olc::PixelGameEngine *pge) 
        {
            int nx, ny;
            WorldToScreen(this->position, nx, ny);
            pge->DrawCircle(nx, ny, CircuitElement::getWorldScale());
        }
