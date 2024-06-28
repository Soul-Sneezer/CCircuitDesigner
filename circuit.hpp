#ifndef CC_CIRCUIT
#define CC_CIRCUIT

#include "circuit_elements.hpp"

class Circuit
{
    private:
        int32_t voltageIn;
        int32_t voltageOut;
        std::vector<std::shared_ptr<CircuitElement>> elements;
        void drawElement(std::shared_ptr<CircuitElement> element, olc::PixelGameEngine *pge);
    public:
        void selectElement(olc::PixelGameEngine* pge, float scale);
        void addElem(std::shared_ptr<CircuitElement>& element);
        Circuit();
        Circuit(const Circuit& circuit);
        ~Circuit();

        int32_t getVoltageIn() const;
        int32_t getVoltageOut() const;
        std::vector<std::shared_ptr<CircuitElement>>& getElements(); 
        Circuit& operator=(Circuit& circuit);

        void run();
        void drawCircuit(olc::PixelGameEngine *pge);
        
        friend void swap(Circuit& first, Circuit& second) noexcept;
};

#endif
