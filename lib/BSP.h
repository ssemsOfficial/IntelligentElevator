#ifndef BSP_H
#define BSP_H
    

//



//components
// IR
#define CAPTEUR_IR_ACTIVE                0
#define CAPTEUR_IR_DESACTIVE             1
// IR Driver
#define ASCENSEUR_ETAGE_0_DETECTE        CAPTEUR_IR_ACTIVE
#define ASCENSEUR_ETAGE_0_NON_DETECTE    CAPTEUR_IR_DESACTIVE
#define ASCENSEUR_ETAGE_1_DETECTE        CAPTEUR_IR_ACTIVE
#define ASCENSEUR_ETAGE_1_NON_DETECTE    CAPTEUR_IR_DESACTIVE

//relay
#define RELAY_CLOSED            1
#define RELAY_OPENED            0
#define OPEN_RELAY              1
#define CLOSE_RELAY             0


//L298N
#define TURN_OFF_MOTOR_1        0
#define TURN_ON_MOTOR_1         1
#define MOTOR_1_TURNED_OFF      0
#define MOTOR_1_TURNED_ON       1
#define MOTOR_1_TURNED_SENSE_1  0 
#define MOTOR_1_TURNED_SENSE_2  1
#define MOTOR_2_TURNED_OFF      0
#define MOTOR_2_TURNED_ON       1
#define TURN_OFF_MOTOR_2        0
#define TURN_ON_MOTOR_2         1   
#define MOTOR_2_TURNED_SENSE_1  0 
#define MOTOR_2_TURNED_SENSE_2  1
//Driver L298N
#define DEMARRER_MOTEUR         TURN_OFF_MOTOR_1
#define ARRERTER_MOTEUR         TURN_ON_MOTOR_1
#define MOTEUR_DEMARRE          MOTOR_1_TURNED_ON
#define MOTEUR_ARRERTE          MOTOR_1_TURNED_OFF
#define MONTER_ASCENSEUR        MOTOR_1_TURNED_SENSE_1
#define DESCENDRE_ASCENSEUR     MOTOR_1_TURNED_SENSE_2
#define ASCENSEUR_MONTE TURN    MOTOR_1_TURNED_SENSE_1
#define ASCENSEUR_DESCENT       MOTOR_1_TURNED_SENSE_2
//Lcd

//Lcd Custom char
#define ETAGE_0_CHAR 0
#define ETAGE_1_CHAR 1
#define UP_ANIMATE_1_CHAR 2
#define DOWN_ANIMATE_1_CHAR 11

//BUTTON
#define BOUTTON_MONTER_HAUT_CLICKE          1
#define BOUTTON_MONTER_HAUT_NON_CLICKE      0
#define BOUTTON_DECENDRE_BAS_CLICKE         1
#define BOUTTON_DECENDRE_BAS_NON_CLICKE     0



//inputs
#define pinIRSensorEtage0      36
#define pinIRSensorEtage1      39
#define pinBouttonMonter       34
#define pinBouttonDescendre    35




///outputs
#define pinMotor1Pin1          32
#define pinMotor1Pin2          33
#define pinMotor2Pin1          25
#define pinMotor2Pin2          26


#endif