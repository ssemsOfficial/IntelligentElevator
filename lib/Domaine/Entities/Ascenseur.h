#ifndef ASCENSEUR_H 
#define ASCENSEUR_H


#include <vector>
#include <string>
#include <nlohmann/json.hpp> // JSON library



//#include"Reservation.h"


class Ascenseur
{
public:
    //Reservation _reservation;
    std::string _id;
    std::string _etat;

    //events
    struct systemEvents 
    {
        struct SurBT_Monter
        {
            bool etat = false;
            bool etatPre = false;
        };
        SurBT_Monter surBT_Monter;
        
        struct SurBT_Descendre
        {
            bool etat = false;
            bool etatPre = false;
        };
        SurBT_Descendre surBT_Descendre;
        
        struct SurIR_E0
        {
            bool etat = false;
            bool etatPre = false;
        };
        SurIR_E0 surIR_E0;

        struct SurIR_E1
        {
            bool etat = false;
            bool etatPre = false;
        };
        SurIR_E1 surIR_E1;
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
    systemStates states     = INIT;
    systemStates statesPre  = INIT;

    //actions
    struct systemActions
    {
        struct Moteur
        {
            bool etat    = false;
            bool etatPre = false;
            bool sense    = false;
            bool sensePre = false;
            
        };
        Moteur moteur;
    };
    systemActions actions;

    Ascenseur(){

    }

};

#endif