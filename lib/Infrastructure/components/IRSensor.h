#ifndef IRSENSOR_H
#define IRSENSOR_H

#include<Arduino.h>
#include"../../BSP.h"

class IRSensor
{
private:
    int _pin;
    bool _etat;

public:
    IRSensor(int pin)
     : _pin(pin)
    {
        
    }
    ~IRSensor(){
        
    }
    void init();

    bool estDetecte();

    bool getEtat();
};

void IRSensor::init(){
        pinMode(_pin, INPUT);
        _etat = CAPTEUR_IR_DESACTIVE;
}

bool IRSensor::estDetecte(){
    _etat  = digitalRead(_pin);
    return _etat;
}
bool IRSensor::getEtat(){
    return _etat;
}


#endif