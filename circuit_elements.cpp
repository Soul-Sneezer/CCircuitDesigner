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

        void swap(Transistor& first, Transistor& second) noexcept
        {
            using std::swap;
            swap(first.position, second.position);
            swap(first.voltage, second.voltage);
            swap(first.power, second.power);
            swap(first.in, second.in);
            swap(first.out, second.out);
            swap(first.temperature, second.temperature);
            swap(first.thresholdVoltage, second.thresholdVoltage);
            swap(first.threshold, second.threshold);
        }

        void swap(Resistor& first, Resistor& second) noexcept
        { 
            using std::swap;
            swap(first.position, second.position);
            swap(first.voltage, second.voltage);
            swap(first.power, second.power);
            swap(first.in, second.in);
            swap(first.out, second.out);
            swap(first.temperature, second.temperature);
            swap(first.resistance, second.resistance);
            swap(first.powerDissipation, second.powerDissipation);
        }

        void swap(Source& first, Source& second) noexcept
        {   
            using std::swap;
            swap(first.position, second.position);
            swap(first.voltage, second.voltage);
            swap(first.power, second.power);
            swap(first.in, second.in);
            swap(first.out, second.out);
            swap(first.temperature, second.temperature);
        }

        void swap(Battery& first, Battery& second) noexcept
        {
            using std::swap;
            swap(first.position, second.position);
            swap(first.voltage, second.voltage);
            swap(first.power, second.power);
            swap(first.in, second.in);
            swap(first.out, second.out);
            swap(first.temperature, second.temperature);
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
            :position{element.position},
             voltage{element.voltage},
             power{element.power},
            temperature{element.temperature},
             in{element.in},
             out{element.out}
        {
            element.position = std::make_pair<olc::vf2d, olc::vf2d>({0,0}, {0,0});
            element.voltage = 0;
            element.power = 0;
            element.in = nullptr;
            element.out = nullptr;
            element.temperature = 273;
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
            this->position = element.getPosition();
            this->voltage = element.getVoltage();
            this->power = element.getPower();
            this->in = element.getIn();
            this->out = element.getOut();
            this->temperature = element.getTemperature();

            element.position = std::make_pair<olc::vf2d, olc::vf2d>({0, 0}, {0, 0});
            element.voltage = 0;
            element.power = 0;
            element.in = nullptr;
            element.out = nullptr;
            element.temperature = 273;

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

        // cppcheck-suppress unusedFunction
        CircuitElement* CircuitElement::toBaseClass()
        {
            return dynamic_cast<CircuitElement*>(this);
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

        CableNode::CableNode(CableNode&& node) : CircuitElement(node),
             inputs{node.inputs},
             outputs{node.outputs}
        {
            node.inputs.clear();
            node.outputs.clear();
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
            this->inputs = element.getInputs();
            this->outputs = element.getOutputs();
            
            this->position = element.getPosition();
            this->voltage = element.getVoltage();
            this->power = element.getPower();
            this->in = element.getIn();
            this->out = element.getOut();
            this->temperature = element.getTemperature();

            element.inputs.clear();
            element.outputs.clear();

            element.position = std::make_pair<olc::vf2d, olc::vf2d>({0, 0}, {0, 0});
            element.voltage = 0;
            element.power = 0;
            element.in = nullptr;
            element.out = nullptr;
            element.temperature = 273;

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

        Transistor::Transistor(Transistor&& transistor) : CircuitElement(transistor),
            thresholdVoltage(transistor.thresholdVoltage),
            threshold(transistor.threshold)
        {
            transistor.threshold = 0;
            transistor.thresholdVoltage = 0;
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
            this->threshold = transistor.getThreshold();
            this->thresholdVoltage = transistor.getThresholdVoltage();
            
            this->position = transistor.getPosition();
            this->voltage = transistor.getVoltage();
            this->power = transistor.getPower();
            this->in = transistor.getIn();
            this->out = transistor.getOut();
            this->temperature = transistor.getTemperature();

            transistor.threshold = 0;
            transistor.thresholdVoltage = 0;
            transistor.position = std::make_pair<olc::vf2d, olc::vf2d>({0, 0}, {0, 0});
            transistor.voltage = 0;
            transistor.power = 0;
            transistor.in = nullptr;
            transistor.out = nullptr;
            transistor.temperature = 273;

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

        Resistor::Resistor(Resistor&& r) : CircuitElement(r),
            resistance(r.resistance),
            powerDissipation(r.powerDissipation)
        {
            r.resistance = 0;
            r.powerDissipation = 0;
        }

        Resistor::~Resistor()
        {
        }

        std::ostream& Resistor::operator<<(std::ostream& os)
        {
            os<<"Resistance: "<<this->getResistance()<<"    Power that can be safely dissipated: "<<this->getPowerDissipation()<<"\n";
            os << "Voltage: "<<this->getVoltage() <<"    Power: " <<this->getPower() <<"    Temperature: "<<this->getTemperature()<<"\n";

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
            this->resistance = r.getResistance();
            this->powerDissipation = r.getPowerDissipation();

            this->position = r.getPosition();
            this->voltage = r.getVoltage();
            this->power = r.getPower();
            this->in = r.getIn();
            this->out = r.getOut();
            this->temperature = r.getTemperature();

            r.resistance = 0;
            r.powerDissipation = 0;
            r.position = std::make_pair<olc::vf2d, olc::vf2d>({ 0, 0 }, { 0, 0});
            r.voltage = 0;
            r.power = 0;
            r.in = nullptr;
            r.out = nullptr;
            r.temperature = 273;

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
            this->length = 0;
            this->resistance = 0;
            this->reverse = false;
        }

        Cable::Cable(std::pair<olc::vf2d, olc::vf2d> pos, \
                const int32_t voltage, const int32_t power, \
                CircuitElement* in, CircuitElement* out, \
                const int32_t resistance, const bool reverse, const int32_t length, const uint32_t temperature)
        {
            this->position = pos;

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
            this->resistance = c.resistance;
            this->reverse = c.reverse;
            this->length = c.length;
        }

        Cable::Cable(Cable&& c) : CircuitElement(c),
            resistance(c.resistance),
            reverse(c.reverse),
            length(c.length)
        {
            c.resistance = 0;
            c.reverse = false;
            c.length = 0;
        }

        Cable::~Cable()
        {
        }

        std::ostream& Cable::operator<<(std::ostream& os)
        {
            os <<"Resistance: "<<this->getResistance()<<"   Voltage: "<<this->getVoltage()<<"    Power: " <<this->getPower()<<"    Temperature: "<<this->getTemperature()<<"\n";

            return os;
        }

        int32_t Cable::getResistance() const { return this->resistance; }
        Switch Cable::getSwitch() const { return this->circuitSwitch; }
        bool Cable::getFlowDirection() const { return this->reverse; }
        int32_t Cable::getLength() const { return this->length; }
     
        Cable& Cable::operator=(const Cable& cable) 
        {
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
            this->reverse = cable.getFlowDirection();
            this->resistance = cable.getResistance();
            this->length = cable.getLength();

            this->position = cable.getPosition();
            this->voltage = cable.getVoltage();
            this->power = cable.getPower();
            this->in = cable.getIn();
            this->out = cable.getOut();
            this->temperature = cable.getTemperature();

            cable.reverse = false;
            cable.resistance = 0;
            cable.length = 0;
            cable.position = std::make_pair<olc::vf2d, olc::vf2d>({ 0, 0 }, { 0, 0 });
            cable.voltage = 0;
            cable.power = 0;
            cable.in = nullptr;
            cable.out = nullptr;
            cable.temperature = 273;

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

        Source::Source(Source&& s) : CircuitElement(s)
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
            this->position = s.getPosition();
            this->voltage = s.getVoltage();
            this->power = s.getPower();
            this->in = s.getIn();
            this->out = s.getOut();
            this->temperature = s.getTemperature();

            s.position = std::make_pair<olc::vf2d, olc::vf2d>({ 0,0 }, { 0,0 });
            s.voltage = 0;
            s.power = 0;
            s.in = nullptr;
            s.out = nullptr;
            s.temperature = 273;

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

        Battery::Battery(Battery&& b) : CircuitElement(b),
            capacity(b.capacity)
        {
            b.capacity = 0;
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
            this->capacity = b.getCapacity();
            
            this->position = b.getPosition();
            this->voltage = b.getVoltage();
            this->power = b.getPower();
            this->in = b.getIn();
            this->out = b.getOut();
            this->temperature = b.getTemperature();

            b.capacity = 0;
            b.position = std::make_pair<olc::vf2d, olc::vf2d>({ 0,0 }, { 0,0 });
            b.voltage = 0;
            b.power = 0;
            b.in = nullptr;
            b.out = nullptr;
            b.temperature = 273;

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
