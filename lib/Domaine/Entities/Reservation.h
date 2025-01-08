#ifndef RESERVATION_H
#define RESERVATION_H

#include<string>


class Reservation
{
public:
    std::string etat;
    std::string etage;
    //events
    struct systemEvents 
    {
        bool surBT0 = false;
        bool surBT1 = false;
        bool surIR0 = false;
        bool surIR1 = false;
    };
    systemEvents events;
    
    //system states
    enum systemStates
    {
        INIT,
        ASCENSEUR_EST_EN_ETAGE_0,
        ASCENSEUR_EST_EN_ETAGE_1,
        ASCENSEUR_EST_MONTANT,
        ASCENSEUR_EST_DESENDANT,
    };
    systemStates states= INIT;

    //actions
    struct systemActions
    {
        struct Moteur
        {
            bool etat = false;
            bool sense = false;
        };
        
        Moteur moteur;
    };
    systemActions actions;

    Reservation()
    {}
    ~Reservation()
    {}
};


#endif