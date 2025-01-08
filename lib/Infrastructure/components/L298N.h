#ifndef L298N_H
#define L298N_H

#include <Arduino.h>
#include"../../BSP.h"

class L298N
{
private:
    int _motor1Pin1;
    int _motor1Pin2;
    int _motor2Pin1;
    int _motor2Pin2;
    
    bool _motor1State;
    bool _motor2State;
    bool _motor1Sensor;
    bool _motor2Sensor;
    
public:
    L298N(int motor1Pin1, int motor1Pin2):
        _motor1Pin1(motor1Pin1), _motor1Pin2(motor1Pin2) 
    {
        
    }
    ~L298N(){
        
    }
    void init();
    void turnOffMotor1();
    void turnOnMotor1Sense1();
    void turnOnMotor1Sense2();
    void turnOffMotor2();
    void turnOnMotor2Sense1();
    void turnOnMotor2Sense2();
};

void L298N::init(){
    pinMode(_motor1Pin1, OUTPUT);
    pinMode(_motor1Pin2, OUTPUT);
    pinMode(_motor2Pin1, OUTPUT);
    pinMode(_motor2Pin2, OUTPUT);
    digitalWrite(_motor1Pin1, TURN_OFF_MOTOR_1);
    digitalWrite(_motor1Pin2, TURN_OFF_MOTOR_1);
    digitalWrite(_motor2Pin1, TURN_OFF_MOTOR_2);
    digitalWrite(_motor2Pin2, TURN_OFF_MOTOR_2);
    _motor1State = MOTOR_1_TURNED_ON;
    _motor2State = MOTOR_1_TURNED_ON;
}

void L298N::turnOffMotor1(){
    digitalWrite(_motor1Pin1, TURN_OFF_MOTOR_1);
    digitalWrite(_motor1Pin2, TURN_OFF_MOTOR_1);
}
void L298N::turnOnMotor1Sense1(){
    digitalWrite(_motor1Pin1, TURN_ON_MOTOR_1);
    digitalWrite(_motor1Pin2, TURN_OFF_MOTOR_1);
}
void L298N::turnOnMotor1Sense2(){
    digitalWrite(_motor1Pin1, TURN_OFF_MOTOR_1);
    digitalWrite(_motor1Pin2, TURN_ON_MOTOR_1);
}

void L298N::turnOffMotor2(){
    digitalWrite(_motor2Pin1, TURN_OFF_MOTOR_2);
    digitalWrite(_motor2Pin2, TURN_OFF_MOTOR_2);
}
void L298N::turnOnMotor2Sense1(){
    digitalWrite(_motor2Pin1, TURN_ON_MOTOR_2);
    digitalWrite(_motor2Pin2, TURN_OFF_MOTOR_2);
}
void L298N::turnOnMotor2Sense2(){
    digitalWrite(_motor2Pin1, TURN_OFF_MOTOR_2);
    digitalWrite(_motor2Pin2, TURN_ON_MOTOR_2);
}

#endif