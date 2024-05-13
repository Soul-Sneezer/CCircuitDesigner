#include "circuit.hpp"
        void Circuit::addElementToCircuit(std::shared_ptr<CircuitElement>& element)
        {
            elements.push_back(element);
        }
        Circuit::Circuit()
        {
            this->voltageIn = 0;
            this->voltageOut = 0;        
        }

        Circuit::Circuit(const Circuit& circuit)
        {
            this->voltageIn = circuit.voltageIn;
            this->voltageOut = circuit.voltageOut;
        }

        Circuit::~Circuit()
        {

        }

        std::ostream& operator<<(std::ostream& os, const Circuit& el)
        {
            os<<"Voltage in: "<<el.getVoltageIn()<<"  Voltage out: "<<el.getVoltageOut()<<"\n";
            return os;
        }

        int32_t Circuit::getVoltageIn() const{ return this->voltageIn; };
        int32_t Circuit::getVoltageOut() const{ return this->voltageOut; };
        std::vector<std::shared_ptr<CircuitElement>>& Circuit::getElements() { return this->elements; };
        Circuit& Circuit::operator=(const Circuit& circuit)
        {
            this->voltageIn = circuit.getVoltageIn();
            this->voltageOut = circuit.getVoltageOut();

            return *this;
        }

        void Circuit::run()
        {
        }
