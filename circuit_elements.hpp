#ifndef CC_CIRCUIT_ELEMENTS
#define CC_CIRCUIT_ELEMENTS

#include <olcPixelGameEngine.h>

enum class ElementType 
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
    private:
        static float& worldScale()
        {
            static float v1 = 0.0f;

            return v1;
        }
        static olc::vf2d& worldOffset()
        {
            static olc::vf2d v2 = { 0.0f, 0.0f };

            return v2;
        }

    protected:
        olc::vi2d position = {0, 0};

        
        int32_t voltage;
        int32_t power;
        uint32_t temperature; // useless for now

        CircuitElement* in; // prev element
        CircuitElement* out; // next element
    public:
        CircuitElement();
        explicit CircuitElement(const int32_t posX, const int32_t posY, const int32_t voltage, const int32_t power = 0, \
                CircuitElement* in = NULL, CircuitElement* out = NULL, \
                const uint32_t temperature = 273);
        CircuitElement(const CircuitElement& element);
        CircuitElement(CircuitElement&& element);
        virtual ~CircuitElement() = 0;

        olc::vi2d getPosition() const; 
        int32_t getVoltage() const; 
        int32_t getPower() const; 
        int32_t getTemperature() const; 
        CircuitElement* getIn() const; 
        CircuitElement* getOut() const;
      
        static olc::vf2d getWorldOffset() { return worldOffset(); };
        static float getWorldScale() { return worldScale(); };
        static void setWorldScale(const float scale)
        {
            worldScale() = scale;
        }
        static void setWorldOffset(const olc::vf2d offset)
        {
            worldOffset() = offset;
        }

        CircuitElement& operator=(const CircuitElement& element);
        CircuitElement& operator=(CircuitElement&& element);
        std::ostream& operator<<(std::ostream& os); 

        void WorldToScreen(const olc::vf2d& v, int& screenX, int& screenY);
        virtual void changeValue(int32_t& value) = 0;
        virtual void drawYourself(olc::PixelGameEngine *pge) = 0;
};

class CableNode : public CircuitElement
{
    private:
        std::vector<std::shared_ptr<CircuitElement>> inputs;
        std::vector<std::shared_ptr<CircuitElement>> outputs;
    public:
        CableNode();   
        explicit CableNode(const int32_t posX, const int32_t posY);
        CableNode(const CableNode& node); 
        CableNode(CableNode&& node);
        ~CableNode();

        std::vector<std::shared_ptr<CircuitElement>> getInputs() const; 
        std::vector<std::shared_ptr<CircuitElement>> getOutputs() const;

        CableNode& operator=(const CableNode& element);
        CableNode& operator=(CableNode&& eleemnt);
        std::ostream& operator<<(std::ostream& os); 

        void changeValue(int32_t& value) override;
        void drawYourself(olc::PixelGameEngine *pge) override;
};

class Transistor : public CircuitElement
{
    private:
        int32_t thresholdVoltage;
        int32_t threshold;
    public:
        Transistor(); 
        explicit Transistor(const int32_t posX, const int32_t posY, const int32_t voltage = 0, const int32_t power = 0, \
                CircuitElement* in = NULL, CircuitElement* out = NULL,\
                const int32_t threshold = 0, const int32_t thresholdVoltage = 0, const uint32_t temperature = 273);
        Transistor(const Transistor& transistor);
        Transistor(Transistor&& transistor);
        ~Transistor();

        int32_t getThreshold() const; 
        int32_t getThresholdVoltage() const; 

        Transistor& operator=(const Transistor& transistor);
        Transistor& operator=(Transistor&& transistor);
        std::ostream& operator<<(std::ostream& os); 

        void changeValue(int32_t& value) override;
        void drawYourself(olc::PixelGameEngine *pge) override;
};

class Resistor : public CircuitElement
{
    private:
        int32_t resistance;
        int32_t powerDissipation;
        int32_t tolerance;
    public:
        Resistor();
        explicit Resistor(const int32_t posX, const int32_t posY,\
                const int32_t voltage = 0, const int32_t power = 0, CircuitElement* in = NULL, CircuitElement* out = NULL, \
                const int32_t resistance = 0, const int32_t powerDissipation = 0, const int32_t tolerance = 0, const uint32_t temperature = 273);
        Resistor(const Resistor& r);
        Resistor(Resistor&& r);
        virtual ~Resistor();

        int32_t getResistance() const;
        int32_t getPowerDissipation() const; 
        int32_t getTolerance() const; 

        Resistor& operator=(const Resistor& r);
        Resistor& operator=(Resistor&& r);
        std::ostream& operator<<(std::ostream& os); 

        void changeValue(int32_t& value) override;
        void drawYourself(olc::PixelGameEngine *pge) override;
};

class Switch
{
    private:
        bool open;
        bool active;
    public:
        Switch();
        Switch(const Switch& s); 
        ~Switch();

        bool isOpen();
        bool isActive();
        void activate();
        void deactivate();
        void openSwitch();
        void closeSwitch();
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
        Cable();
        explicit Cable(const int32_t posX, const int32_t posY, const olc::vi2d end = {0, 0}, \
                const int32_t voltage = 0, const int32_t power = 0, \
                CircuitElement* in = NULL,CircuitElement* out = NULL, \
                const int32_t resistance = 0, const bool reverse = false, const int32_t length = 0, const uint32_t temperature = 273);
        Cable(const Cable& c);
        Cable(Cable&& c);
        ~Cable();

        olc::vi2d getEnd() const; 
        int32_t getResistance() const; 
        Switch getSwitch() const; 
        bool getFlowDirection() const; 
        int32_t getLength() const; 

        Cable& operator=(const Cable& cable);
        Cable& operator=(Cable&& cable);
        std::ostream& operator<<(std::ostream& os); 

        void changeValue(int32_t& value) override;
        void drawYourself(olc::PixelGameEngine *pge) override;
};

class Source : public CircuitElement
{
    private:
    public:
        Source();
        Source(int32_t posX, int32_t posY, int32_t voltage = 0, int32_t power = 0, \
                CircuitElement* in = NULL, CircuitElement* out = NULL,\
                uint32_t temperature = 273);
        Source(const Source& s);
        Source(Source&& s);
        ~Source();

        Source& operator=(const Source& s);
        Source& operator=(Source&& s);
        std::ostream& operator<<(std::ostream& os); 

        void changeValue(int32_t& value) override;
        void drawYourself(olc::PixelGameEngine *pge) override;
};


class Battery : public CircuitElement
{
    private:
        uint32_t capacity;
    public:
        Battery();
        explicit Battery(const int32_t posX, const int32_t posY, const int32_t voltage = 0, const int32_t power = 0, \
                CircuitElement* in = NULL, CircuitElement* out = NULL, \
                const uint32_t capacity = 0, const uint32_t temperature = 273);
        Battery(const Battery& b);
        Battery(Battery&& b);
        ~Battery();

        int32_t getCapacity() const; 

        Battery& operator=(const Battery& b);
        Battery& operator=(Battery&& b);
        std::ostream& operator<<(std::ostream& os); 

        void changeValue(int32_t& value) override;
        void drawYourself(olc::PixelGameEngine *pge) override;
};

#endif
