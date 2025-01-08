#ifndef RELAY_H
#define RELAY_H

#include<Arduino.h>
#include"../../BSP.h"

class Relay
{
private:
    int _pin;
    bool _state;

public:
    Relay(int pin):
        _pin(pin)
    {}
    ~Relay()
    {}
    void init();
    void open();
    void close();
};

void Relay::init(){
        pinMode(_pin, OUTPUT);
        _state = RELAY_OPENED;
}

void Relay::close(){
    digitalWrite(_pin, CLOSE_RELAY);
    _state = RELAY_CLOSED;
}

void Relay::open(){
    digitalWrite(_pin, OPEN_RELAY);
    _state = RELAY_OPENED;
}


#endif