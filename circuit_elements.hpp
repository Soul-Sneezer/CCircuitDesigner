#ifndef CC_CIRCUIT_ELEMENTS
#define CC_CIRCUIT_ELEMENTS

#include <olcPixelGameEngine.h>
#include "debug.hpp"

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

void allocSprites();
template<typename T> void allocSprite(const char* path);

class CircuitElement
{
    private:
        friend void swap(CircuitElement& first, CircuitElement& second) noexcept;

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
        std::pair<olc::vf2d, olc::vf2d> position = std::make_pair<olc::vf2d, olc::vf2d>({0, 0}, {0, 0});
        static std::shared_ptr<olc::Sprite> sprite; 
        int32_t voltage;
        int32_t power;
        uint32_t temperature; // useless for now

        std::shared_ptr<CircuitElement> in; // prev element
        std::shared_ptr<CircuitElement> out; // next element
    
    public:
        template<typename T> friend void allocSprite(const char* path);

        CircuitElement();
        explicit CircuitElement(std::pair<olc::vf2d, olc::vf2d> pos, const int32_t voltage, const int32_t power = 0, \
                std::shared_ptr<CircuitElement> in = nullptr, std::shared_ptr<CircuitElement> out = nullptr, \
                const uint32_t temperature = 273);
        CircuitElement(const CircuitElement& element);
        CircuitElement(CircuitElement&& element);
        virtual ~CircuitElement() = 0;

        virtual std::shared_ptr<CircuitElement> clone() = 0;

        std::pair<olc::vf2d, olc::vf2d> getPosition() const; 
        int32_t getVoltage() const; 
        int32_t getPower() const; 
        int32_t getTemperature() const; 
        std::shared_ptr<CircuitElement> getIn() const; 
        std::shared_ptr<CircuitElement> getOut() const;
      
        static olc::vf2d getWorldOffset() { return worldOffset(); };
        static float getWorldScale() { return worldScale(); };
        static std::shared_ptr<olc::Sprite> getSprite() { return sprite; };

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
        friend std::ostream& operator<<(std::ostream& os, const CircuitElement& element); 

        void WorldToScreen(const olc::vf2d& v, int& screenX, int& screenY);

        virtual void drawYourself(olc::PixelGameEngine *pge) = 0;
};

class Transistor : public CircuitElement
{
    private:
        friend void swap(Transistor& first, Transistor& second) noexcept;

        int32_t thresholdVoltage;
        int32_t threshold;
    public:
        Transistor(); 
        explicit Transistor(std::pair<olc::vf2d, olc::vf2d> pos, const int32_t voltage = 0, const int32_t power = 0, \
                std::shared_ptr<CircuitElement> in = nullptr, std::shared_ptr<CircuitElement> out = nullptr,\
                const int32_t threshold = 0, const int32_t thresholdVoltage = 0, const uint32_t temperature = 273);
        Transistor(const Transistor& transistor);
        Transistor(Transistor&& transistor);
        ~Transistor();
        
        std::shared_ptr<CircuitElement> clone() override;
        int32_t getThreshold() const; 
        int32_t getThresholdVoltage() const; 

        Transistor& operator=(const Transistor& transistor);
        Transistor& operator=(Transistor&& transistor);
        friend std::ostream& operator<<(std::ostream& os, const Transistor& transistor); 

        void drawYourself(olc::PixelGameEngine *pge) override;
};

class Resistor : public CircuitElement
{
    private:
        friend void swap(Resistor& first, Resistor& second) noexcept;

        int32_t resistance;
        int32_t powerDissipation;
    public:
        Resistor();
        explicit Resistor(std::pair<olc::vf2d, olc::vf2d> pos,\
                const int32_t voltage = 0, const int32_t power = 0, std::shared_ptr<CircuitElement> in = nullptr, std::shared_ptr<CircuitElement> out = nullptr, \
                const int32_t resistance = 0, const int32_t powerDissipation = 0, const uint32_t temperature = 273);
        Resistor(const Resistor& r);
        Resistor(Resistor&& r);
        virtual ~Resistor();
       
        std::shared_ptr<CircuitElement> clone() override;
        int32_t getResistance() const;
        int32_t getPowerDissipation() const; 

        Resistor& operator=(const Resistor& r);
        Resistor& operator=(Resistor&& r);
        friend std::ostream& operator<<(std::ostream& os, const Resistor& r); 

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

class CableNode 
{
    private:
        friend void swap(CableNode& first, CableNode& second) noexcept;

        std::vector<std::shared_ptr<CircuitElement>> inputs;
        std::vector<std::shared_ptr<CircuitElement>> outputs;
    public:
        CableNode();   
        CableNode(const CableNode& node); 
        CableNode(CableNode&& node);
        ~CableNode();

        std::vector<std::shared_ptr<CircuitElement>> getInputs() const; 
        std::vector<std::shared_ptr<CircuitElement>> getOutputs() const;

        CableNode& operator=(const CableNode& element);
        CableNode& operator=(CableNode&& element);
        friend std::ostream& operator<<(std::ostream& os, const CableNode& element); 

        void addElementToInputs(std::shared_ptr<CircuitElement> elem);
        void addElementToOutputs(std::shared_ptr<CircuitElement> elem);
};

class Cable : public CircuitElement
{
    private:
        friend void swap(Cable& first, Cable& second) noexcept;

        int32_t resistance;
        bool reverse;
        Switch circuitSwitch;
    public:
        Cable();
        explicit Cable(std::pair<olc::vf2d, olc::vf2d> pos, \
                const int32_t voltage = 0, const int32_t power = 0, \
                std::shared_ptr<CircuitElement> in = nullptr, std::shared_ptr<CircuitElement> out = nullptr, \
                const int32_t resistance = 0, const bool reverse = false, const uint32_t temperature = 273);
        Cable(const Cable& c);
        Cable(Cable&& c);
        ~Cable();

        std::shared_ptr<CircuitElement> clone() override;
        int32_t getResistance() const; 
        Switch getSwitch() const; 
        bool getFlowDirection() const; 
        int32_t getLength() const; 

        Cable& operator=(const Cable& cable);
        Cable& operator=(Cable&& cable);
        friend std::ostream& operator<<(std::ostream& os, const Cable& cable); 

        void drawYourself(olc::PixelGameEngine *pge) override;
};

class Source : public CircuitElement
{
    private:
        friend void swap(Source& first, Source& second) noexcept;
    public:
        Source();
        explicit Source(std::pair<olc::vf2d, olc::vf2d> pos, int32_t voltage = 0, int32_t power = 0, \
                std::shared_ptr<CircuitElement> in = nullptr, std::shared_ptr<CircuitElement> out = nullptr,\
                uint32_t temperature = 273);
        Source(const Source& s);
        Source(Source&& s);
        ~Source();
       
        std::shared_ptr<CircuitElement> clone() override;
        Source& operator=(const Source& s);
        Source& operator=(Source&& s);
        friend std::ostream& operator<<(std::ostream& os, const Source& s); 

        void drawYourself(olc::PixelGameEngine *pge) override;
};


class Battery : public CircuitElement
{
    private:
        friend void swap(Battery& first, Battery& second) noexcept;

        uint32_t capacity;
    public:
        Battery();
        explicit Battery(std::pair<olc::vf2d, olc::vf2d> pos, const int32_t voltage = 0, const int32_t power = 0, \
                std::shared_ptr<CircuitElement> in = nullptr, std::shared_ptr<CircuitElement> out = nullptr, \
                const uint32_t capacity = 0, const uint32_t temperature = 273);
        Battery(const Battery& b);
        Battery(Battery&& b);
        ~Battery();

        std::shared_ptr<CircuitElement> clone() override;
        int32_t getCapacity() const; 

        Battery& operator=(const Battery& b);
        Battery& operator=(Battery&& b);
        friend std::ostream& operator<<(std::ostream& os, const Battery& b); 

        void drawYourself(olc::PixelGameEngine *pge) override;
};

#endif
