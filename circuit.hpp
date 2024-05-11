#ifndef CC_CIRCUIT
#define CC_CIRCUIT

#include "circuit_elements.hpp"

class Circuit
{
    private:
        int32_t voltageIn;
        int32_t voltageOut;
        std::vector<CircuitElement*> elements;
    public:
        void addElementToCircuit(CircuitElement* element);

        Circuit();
        Circuit(const Circuit& circuit);
        ~Circuit();

        int32_t getVoltageIn() const;
        int32_t getVoltageOut() const;
        std::vector<CircuitElement*>& getElements(); 
        Circuit& operator=(const Circuit& circuit);

        void run();
};

#endif
