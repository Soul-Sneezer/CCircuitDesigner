#include "circuit_elements.hpp"

        SpriteAllocFailed::SpriteAllocFailed(const char* message) throw() : std::runtime_error(message)
        {

        }

        void swap(CircuitElement& first, CircuitElement& second) noexcept
        {
            using std::swap;
            swap(first.position, second.position);
            swap(first.voltage, second.voltage);
            swap(first.power, second.power);
            swap(first.in, second.in);
            swap(first.out, second.out);
            swap(first.temperature, second.temperature);
        }
        
        void swap(Cable& first, Cable& second) noexcept
        {
            using std::swap;
            swap(static_cast<CircuitElement&>(first), static_cast<CircuitElement&>(second));
            swap(first.resistance, second.resistance);
            swap(first.reverse, second.reverse);
            swap(first.circuitSwitch, second.circuitSwitch);
        }

        void swap(Transistor& first, Transistor& second) noexcept
        {
            using std::swap;
            swap(static_cast<CircuitElement&>(first), static_cast<CircuitElement&>(second));
            swap(first.thresholdVoltage, second.thresholdVoltage);
            swap(first.threshold, second.threshold);
        }

        void swap(Resistor& first, Resistor& second) noexcept
        { 
            using std::swap;
            swap(static_cast<CircuitElement&>(first), static_cast<CircuitElement&>(second));
            swap(first.resistance, second.resistance);
            swap(first.powerDissipation, second.powerDissipation);
        }

        void swap(Source& first, Source& second) noexcept
        {   
            using std::swap;
            swap(static_cast<CircuitElement&>(first), static_cast<CircuitElement&>(second));
        }

        void swap(Battery& first, Battery& second) noexcept
        {
            using std::swap;
            swap(static_cast<CircuitElement&>(first), static_cast<CircuitElement&>(second));
            swap(first.capacity, second.capacity);
        }
        CircuitElement::CircuitElement()
        {
            this->voltage = 0;
            this->power = 0;
            this->in = NULL;
            this->out = NULL;
            this->temperature = 273;
        }

        CircuitElement::CircuitElement(std::pair<olc::vf2d, olc::vf2d> pos, const int32_t voltage, const int32_t power, \
                CircuitElement* in, CircuitElement* out, \
                const uint32_t temperature)
        {
            this->position = pos;

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
            : CircuitElement()
        {
            swap(*this, element);
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
            swap(*this, element);

            return *this;
        }

        CircuitElement::~CircuitElement()
        {
        }

        std::pair<olc::vf2d, olc::vf2d> CircuitElement::getPosition() const { return this->position; }
        int32_t CircuitElement::getVoltage() const { return this->voltage; }
        int32_t CircuitElement::getPower() const { return this->power; }
        int32_t CircuitElement::getTemperature() const { return this->temperature; }
        CircuitElement* CircuitElement::getIn() const { return this->in; }
        CircuitElement* CircuitElement::getOut() const { return this->out; }

        std::ostream& operator<<(std::ostream& os, CircuitElement& el) 
        {
            os << "Voltage: "<<el.voltage <<"    Power: " <<el.power <<"    Temperature: "<<el.temperature<<"\n";
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

        CableNode::CableNode(std::pair<olc::vf2d, olc::vf2d> pos)
        {
            this->position = pos;
        }

        CableNode::CableNode(const CableNode& node) : CircuitElement(node)
        {
            this->inputs = node.inputs;
            this->outputs = node.outputs;
        }

        CableNode::CableNode(CableNode&& node) : CableNode()
        {
            swap(*this, node);
        }

        CableNode::~CableNode()
        {

        }

        std::ostream& operator<<(std::ostream& os, CableNode& node) 
        {
            os<<node.inputs.size()<<" "<<node.outputs.size();
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
            swap(*this, element);

            return *this;

        }

        void CableNode::drawYourself(olc::PixelGameEngine *pge) 
        {
            int coordX1, coordX2, coordY1, coordY2;
            WorldToScreen(this->position.first, coordX1, coordY1);
            WorldToScreen(this->position.second, coordX2, coordY2);
            pge->DrawLine(coordX1, coordY1, coordX2, coordY2, olc::WHITE);
        }
 
        Transistor::Transistor() 
        {
            this->threshold = 0;
            this->thresholdVoltage = 0;
        }

        Transistor::Transistor(std::pair<olc::vf2d, olc::vf2d> pos, const int32_t voltage, const int32_t power, \
                CircuitElement* in, CircuitElement* out,\
                const int32_t threshold, const int32_t thresholdVoltage, const uint32_t temperature)
        {
            this->position = pos;

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

        Transistor::Transistor(Transistor&& transistor) : Transistor()
        {
            swap(*this, transistor);
        }

        Transistor::~Transistor()
        {
        }

        std::ostream& operator<<(std::ostream& os, Transistor& t)
        {
            os<<"Threshold value: "<<t.threshold<<"    Threshold voltage: "<<t.thresholdVoltage<<"\n";
            os << "Voltage: "<<t.voltage <<"    Power: " <<t.power <<"    Temperature: "<<t.temperature<<"\n";

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
            swap(*this, transistor);
            return *this;
        }

        void Transistor::drawYourself(olc::PixelGameEngine *pge) 
        {
            int coordX1, coordX2, coordY1, coordY2;
            WorldToScreen(this->position.first, coordX1, coordY1);
            WorldToScreen(this->position.second, coordX2, coordY2);
            pge->DrawLine(coordX1, coordY1, coordX2, coordY2, olc::WHITE);

           // pge->DrawSprite(nx - CircuitElement::getWorldScale() / 2, ny - CircuitElement::getWorldScale() / 2, Transistor::getSprite(), CircuitElement::getWorldScale() / 30);
        }

        Resistor::Resistor()
        {
            this->resistance = 0;
            this->powerDissipation = 0;
        }

        Resistor::Resistor(std::pair<olc::vf2d, olc::vf2d> pos,\
                const int32_t voltage, const int32_t power, CircuitElement* in, CircuitElement* out, \
                const int32_t resistance, const int32_t powerDissipation , const uint32_t temperature)
        {
            this->position = pos;

            this->voltage = voltage;
            this->power = power;

            this->in = in;
            this->out = out;

            this->resistance = resistance;
            this->powerDissipation = powerDissipation;

            this->temperature = temperature;
        }

        Resistor::Resistor(const Resistor& r) : CircuitElement(r) 
        {
            this->resistance = r.resistance;
            this->powerDissipation = r.powerDissipation;
        }

        Resistor::Resistor(Resistor&& r) : CircuitElement()
        {
            swap(*this, r);
        }

        Resistor::~Resistor()
        {
        }

        std::ostream& operator<<(std::ostream& os, Resistor& r)
        {
            os<<"Resistance: "<<r.resistance<<"    Power that can be safely dissipated: "<<r.powerDissipation<<"\n";
            os << "Voltage: "<<r.voltage <<"    Power: " <<r.power <<"    Temperature: "<<r.temperature<<"\n";

            return os; 
        }

        int32_t Resistor::getResistance() const { return this->resistance; }
        int32_t Resistor::getPowerDissipation() const { return this->powerDissipation; }

        Resistor& Resistor::operator=(const Resistor& r) 
        {
            this->resistance = r.getResistance();
            this->powerDissipation = r.getPowerDissipation();

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
            swap(*this, r);

            return *this;

        }

        void Resistor::drawYourself(olc::PixelGameEngine *pge) 
        {
            int coordX1, coordX2, coordY1, coordY2;
            WorldToScreen(this->position.first, coordX1, coordY1);
            WorldToScreen(this->position.second, coordX2, coordY2);
            pge->DrawLine(coordX1, coordY1, coordX2, coordY2, olc::WHITE);

           // pge->DrawSprite(nx - CircuitElement::getWorldScale() / 2, ny - CircuitElement::getWorldScale() / 2, Resistor::getSprite(), CircuitElement::getWorldScale() / 30);
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
            this->resistance = 0;
            this->reverse = false;
        }

        Cable::Cable(std::pair<olc::vf2d, olc::vf2d> pos, \
                const int32_t voltage, const int32_t power, \
                CircuitElement* in, CircuitElement* out, \
                const int32_t resistance, const bool reverse, const uint32_t temperature)
        {
            this->position = pos;

            this->voltage = voltage;
            this->power = power;

            this->in = in;
            this->out = out;

            this->resistance = resistance;
            this->reverse = reverse;

            this->temperature = temperature;
        }

        Cable::Cable(const Cable& c) : CircuitElement(c) 
        {
            this->resistance = c.resistance;
            this->reverse = c.reverse;
        }

        Cable::Cable(Cable&& c) : Cable()
        {
            swap(*this, c);
        }

        Cable::~Cable()
        {
        }

        std::ostream& operator<<(std::ostream& os, Cable& c)
        {
            os <<"Resistance: "<<c.resistance<<"   Voltage: "<<c.voltage<<"    Power: " <<c.power<<"    Temperature: "<<c.temperature<<"\n";

            return os;
        }

        int32_t Cable::getResistance() const { return this->resistance; }
        Switch Cable::getSwitch() const { return this->circuitSwitch; }
        bool Cable::getFlowDirection() const { return this->reverse; }
     
        Cable& Cable::operator=(const Cable& cable) 
        {
            this->reverse = cable.getFlowDirection();
            this->resistance = cable.getResistance();

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
            swap(*this, cable);

            return *this;

        }
        
        void Cable::drawYourself(olc::PixelGameEngine *pge) 
        {
            int coordX1, coordX2, coordY1, coordY2;
            WorldToScreen(this->position.first, coordX1, coordY1);
            WorldToScreen(this->position.second, coordX2, coordY2);
            pge->DrawLine(coordX1, coordY1, coordX2, coordY2, olc::WHITE);

            //pge->DrawCircle(nx, ny, CircuitElement::getWorldScale());
        }

        Source::Source()
        {
        }

        Source::Source(std::pair<olc::vf2d, olc::vf2d> pos, int32_t voltage, int32_t power, \
                CircuitElement* in, CircuitElement* out,\
                uint32_t temperature)
        {
            this->position = pos;

            this->voltage = voltage;
            this->power = power;

            this->in = in;
            this->out = out;

            this->temperature = temperature;
        }

        Source::Source(const Source& s) : CircuitElement(s) 
        {
        }

        Source::Source(Source&& s) : Source()
        {
            swap(*this, s);
        }
        
        Source::~Source()
        {
        }

        std::ostream& operator<<(std::ostream& os, Source& s)
        {
            os <<"Voltage: "<<s.voltage <<"    Power: " <<s.power <<"    Temperature: "<<s.temperature<<"\n";

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
            swap(*this, s);

            return *this;

        }

        void Source::drawYourself(olc::PixelGameEngine *pge) 
        {
            int coordX1, coordX2, coordY1, coordY2;
            WorldToScreen(this->position.first, coordX1, coordY1);
            WorldToScreen(this->position.second, coordX2, coordY2);
            pge->DrawLine(coordX1, coordY1, coordX2, coordY2, olc::WHITE);

           // pge->DrawSprite(nx - CircuitElement::getWorldScale() / 2, ny - CircuitElement::getWorldScale() / 2, Source::getSprite(), CircuitElement::getWorldScale() / 30);
        }

        Battery::Battery()
        {
            this->capacity = 0;
        }

        Battery::Battery(std::pair<olc::vf2d, olc::vf2d> pos, const int32_t voltage, const int32_t power, \
                CircuitElement* in, CircuitElement* out, \
                const uint32_t capacity, const uint32_t temperature)
        {
            this->position = pos;

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

        Battery::Battery(Battery&& b) : Battery()
        {
            swap(*this, b);
        }
    
        Battery::~Battery()
        {
        }

        std::ostream& operator<<(std::ostream& os, Battery& b)
        {
            os<<"Capacity: "<<b.capacity<<" Voltage: "<<b.voltage<<"\n";
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
            swap(*this, b);

            return *this;

        }

        void Battery::drawYourself(olc::PixelGameEngine *pge) 
        {
            int coordX1, coordX2, coordY1, coordY2;
            WorldToScreen(this->position.first, coordX1, coordY1);
            WorldToScreen(this->position.second, coordX2, coordY2);
            pge->DrawLine(coordX1, coordY1, coordX2, coordY2, olc::WHITE);

           // pge->DrawSprite(nx - CircuitElement::getWorldScale() / 2, ny - CircuitElement::getWorldScale() / 2, Battery::getSprite(), CircuitElement::getWorldScale() / 30);
        }
