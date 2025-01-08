#ifndef DRIVERIR_H
#define DRIVERIR_H

#include"../components/IRSensor.h"

class DriverIR
{
private:
    
public:
    IRSensor &_irsensor;

    DriverIR(IRSensor &irsensor):
        _irsensor(irsensor)
    {

    }
    ~DriverIR()
    {}
    
    bool estPositionDetecte();

};

bool DriverIR::estPositionDetecte(){
    _irsensor.estDetecte();
    return _irsensor.getEtat();
}


#endif