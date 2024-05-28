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

class SpriteAllocFailed : public std::runtime_error
{
    public:
        explicit SpriteAllocFailed(const char* message) throw();
};

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
        
        int32_t voltage;
        int32_t power;
        uint32_t temperature; // useless for now

        CircuitElement* in; // prev element
        CircuitElement* out; // next element
    public:
        CircuitElement();
        explicit CircuitElement(std::pair<olc::vf2d, olc::vf2d> pos, const int32_t voltage, const int32_t power = 0, \
                CircuitElement* in = NULL, CircuitElement* out = NULL, \
                const uint32_t temperature = 273);
        CircuitElement(const CircuitElement& element);
        CircuitElement(CircuitElement&& element);
        virtual ~CircuitElement() = 0;

        std::pair<olc::vf2d, olc::vf2d> getPosition() const; 
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

        CircuitElement* toBaseClass();
        virtual void changeValue(int32_t& value) = 0;
        virtual void drawYourself(olc::PixelGameEngine *pge) = 0;
};

class CableNode : public CircuitElement // work in progress
{
    private:
        std::vector<std::shared_ptr<CircuitElement>> inputs;
        std::vector<std::shared_ptr<CircuitElement>> outputs;
    public:
        CableNode();   
        explicit CableNode(std::pair<olc::vf2d, olc::vf2d> pos);
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
        friend void swap(Transistor& first, Transistor& second) noexcept;

        static olc::Sprite*& sprite()
        {
            static olc::Sprite* v;
            try{
                v = new olc::Sprite("./sprites/transistor.png");
                if(v == nullptr)
                    throw SpriteAllocFailed("Failed to alloc transistor sprite!");

            }
            catch (SpriteAllocFailed const &)
            {
            }

            return v;
        }
        int32_t thresholdVoltage;
        int32_t threshold;
    public:
        Transistor(); 
        explicit Transistor(std::pair<olc::vf2d, olc::vf2d> pos, const int32_t voltage = 0, const int32_t power = 0, \
                CircuitElement* in = NULL, CircuitElement* out = NULL,\
                const int32_t threshold = 0, const int32_t thresholdVoltage = 0, const uint32_t temperature = 273);
        Transistor(const Transistor& transistor);
        Transistor(Transistor&& transistor);
        ~Transistor();
        
        static olc::Sprite* getSprite() { return sprite(); };
        static void setSprite(char* path) { sprite() = new olc::Sprite(path); }

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
        friend void swap(Resistor& first, Resistor& second) noexcept;

        static olc::Sprite*& sprite()
        {
            static olc::Sprite* v;
            try{
                v = new olc::Sprite("./sprites/resistor.png");
                if(v == nullptr)
                    throw SpriteAllocFailed("Faield to alloc resistor sprite!");
            }
            catch(SpriteAllocFailed const &) 
            {
            }
            return v;
        }

        int32_t resistance;
        int32_t powerDissipation;
    public:
        Resistor();
        explicit Resistor(std::pair<olc::vf2d, olc::vf2d> pos,\
                const int32_t voltage = 0, const int32_t power = 0, CircuitElement* in = NULL, CircuitElement* out = NULL, \
                const int32_t resistance = 0, const int32_t powerDissipation = 0, const uint32_t temperature = 273);
        Resistor(const Resistor& r);
        Resistor(Resistor&& r);
        virtual ~Resistor();
        
        static olc::Sprite* getSprite() { return sprite(); };
        static void setSprite(char* path) { sprite() = new olc::Sprite(path); }

        int32_t getResistance() const;
        int32_t getPowerDissipation() const; 

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
        friend void swap(Cable& first, Cable& second) noexcept;

        int32_t resistance;
        bool reverse;
        int32_t length;
        Switch circuitSwitch;
    public:
        Cable();
        explicit Cable(std::pair<olc::vf2d, olc::vf2d> pos, \
                const int32_t voltage = 0, const int32_t power = 0, \
                CircuitElement* in = NULL,CircuitElement* out = NULL, \
                const int32_t resistance = 0, const bool reverse = false, const int32_t length = 0, const uint32_t temperature = 273);
        Cable(const Cable& c);
        Cable(Cable&& c);
        ~Cable();

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
        friend void swap(Source& first, Source& second) noexcept;

        static olc::Sprite*& sprite()
        {
            static olc::Sprite* v;
            try{
                v = new olc::Sprite("./sprites/source.png");
                if(v == nullptr)
                    throw SpriteAllocFailed("Failed to alloc power source sprite!");
            } 
            catch(SpriteAllocFailed const &)
            {
            }

            return v;
        }
    public:
        Source();
        explicit Source(std::pair<olc::vf2d, olc::vf2d> pos, int32_t voltage = 0, int32_t power = 0, \
                CircuitElement* in = NULL, CircuitElement* out = NULL,\
                uint32_t temperature = 273);
        Source(const Source& s);
        Source(Source&& s);
        ~Source();
        
        static olc::Sprite* getSprite() { return sprite(); };
        static void setSprite(char* path) { sprite() = new olc::Sprite(path); }

        Source& operator=(const Source& s);
        Source& operator=(Source&& s);
        std::ostream& operator<<(std::ostream& os); 

        
        void changeValue(int32_t& value) override;
        void drawYourself(olc::PixelGameEngine *pge) override;
};


class Battery : public CircuitElement
{
    private:
        friend void swap(Battery& first, Battery& second) noexcept;

        static olc::Sprite*& sprite()
        {
            static olc::Sprite* v;
            try{
                v = new olc::Sprite("./sprites/battery.png");
                if(v == nullptr)
                    throw SpriteAllocFailed("Failed to alloc battery sprite!");
            }
            catch(SpriteAllocFailed const &)
            {
            }

            return v;
        }
        uint32_t capacity;
    public:
        Battery();
        explicit Battery(std::pair<olc::vf2d, olc::vf2d> pos, const int32_t voltage = 0, const int32_t power = 0, \
                CircuitElement* in = NULL, CircuitElement* out = NULL, \
                const uint32_t capacity = 0, const uint32_t temperature = 273);
        Battery(const Battery& b);
        Battery(Battery&& b);
        ~Battery();

        static olc::Sprite* getSprite() { return sprite(); };
        static void setSprite(char* path) { sprite() = new olc::Sprite(path); }

        int32_t getCapacity() const; 

        Battery& operator=(const Battery& b);
        Battery& operator=(Battery&& b);
        std::ostream& operator<<(std::ostream& os); 

        void changeValue(int32_t& value) override;
        void drawYourself(olc::PixelGameEngine *pge) override;
};

#endif
