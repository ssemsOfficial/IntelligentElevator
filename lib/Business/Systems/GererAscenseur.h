#ifndef GERERASCENSEUR_H
#define GERERASCENSEUR_H


#include<../lib/Infrastructure/drivers/DriverL298N.h>
#include<../lib/Infrastructure/drivers/DriverIR.h>
#include<../lib/Infrastructure/drivers/DriverBoutton.h>
#include<../Domaine/Entities/Ascenseur.h>
//#include<../Presentation/ViewLCD.h>

class GererAscenseur
{
private:
   //entity
    Ascenseur &_ascenseur;
    
    //drivers
    DriverL298N &_driverL298n;
    DriverIR &_driverIR_E0;
    DriverIR &_driverIR_E1;
    DriverBoutton &_driverBoutton_Monter;
    DriverBoutton &_driverBoutton_Descendre; 

    //presentation(View Layer)
    //ViewLCD& _viewLcd;



    
    
public:
    GererAscenseur(Ascenseur &ascenseur, DriverL298N &driverL298n, DriverIR &driverIR_E0, DriverIR &driverIR_E1, DriverBoutton &driverBoutton_Monter, DriverBoutton &driverBoutton_Descendre/*, ViewLCD& viewLcd*/):
        _ascenseur(ascenseur), _driverL298n(driverL298n), _driverIR_E0(driverIR_E0), _driverIR_E1(driverIR_E1), _driverBoutton_Monter(driverBoutton_Monter), _driverBoutton_Descendre(driverBoutton_Descendre)/*, _viewLcd(viewLcd)*/
    {

    }
    ~GererAscenseur(){
        
    }

    void miseAjourEvenement();
    void fsm();
    void miseAjourActions();
};

void GererAscenseur::miseAjourEvenement(){

    Serial.println("-----------------------------");
        Serial.print("ir 0 : ");
        //Serial.println(_driverIR_E0._irsensor.getEtat());
        if(_driverIR_E0._irsensor.getEtat() == ASCENSEUR_ETAGE_0_DETECTE){
            _ascenseur.events.surIR_E0.etatPre = _ascenseur.events.surIR_E0.etat;
            _ascenseur.events.surIR_E0.etat    = ASCENSEUR_ETAGE_0_DETECTE;
            
        }             
        else if(_driverIR_E0._irsensor.getEtat() == ASCENSEUR_ETAGE_0_NON_DETECTE){
            _ascenseur.events.surIR_E0.etatPre = _ascenseur.events.surIR_E0.etat;
            _ascenseur.events.surIR_E0.etat = ASCENSEUR_ETAGE_0_NON_DETECTE;
        }
        Serial.println(_ascenseur.events.surIR_E0.etat);

        Serial.print("ir 1 : ");
        //Serial.println(_driverIR_E1._irsensor.getEtat());
        if(_driverIR_E1._irsensor.getEtat() == ASCENSEUR_ETAGE_1_DETECTE) {
            _ascenseur.events.surIR_E1.etatPre  = _ascenseur.events.surIR_E1.etat;
            _ascenseur.events.surIR_E1.etat     = ASCENSEUR_ETAGE_1_DETECTE;
        }
        else if(_driverIR_E1._irsensor.getEtat() == ASCENSEUR_ETAGE_1_NON_DETECTE){
            _ascenseur.events.surIR_E1.etatPre = _ascenseur.events.surIR_E1.etat;
            _ascenseur.events.surIR_E1.etat = ASCENSEUR_ETAGE_1_NON_DETECTE;
            
        }
        Serial.println(_ascenseur.events.surIR_E1.etat);
        
        
        Serial.print("up   : ");
        //Serial.println(_driverBoutton_Monter._boutton.getEtat());
        if(_driverBoutton_Monter._boutton.getEtat() == BOUTTON_MONTER_HAUT_CLICKE){
            _ascenseur.events.surBT_Monter.etatPre = _ascenseur.events.surBT_Monter.etat;
            _ascenseur.events.surBT_Monter.etat = BOUTTON_MONTER_HAUT_CLICKE;

        }
        else if(_driverBoutton_Monter._boutton.getEtat() == BOUTTON_MONTER_HAUT_NON_CLICKE){
            _ascenseur.events.surBT_Monter.etatPre = _ascenseur.events.surBT_Monter.etat;
            _ascenseur.events.surBT_Monter.etat = BOUTTON_MONTER_HAUT_NON_CLICKE;
        }
        Serial.println(_ascenseur.events.surBT_Monter.etat);
        
        
        Serial.print("dn   : ");
        //Serial.println(_driverBoutton_Descendre._boutton.getEtat());
        if(_driverBoutton_Descendre._boutton.getEtat() == BOUTTON_DECENDRE_BAS_CLICKE)             {
            _ascenseur.events.surBT_Descendre.etatPre = _ascenseur.events.surBT_Descendre.etat;
            _ascenseur.events.surBT_Descendre.etat = BOUTTON_DECENDRE_BAS_CLICKE;
        }
        else if(_driverBoutton_Descendre._boutton.getEtat() == BOUTTON_DECENDRE_BAS_NON_CLICKE) {
            _ascenseur.events.surBT_Descendre.etatPre = _ascenseur.events.surBT_Descendre.etat;
            _ascenseur.events.surBT_Descendre.etat = BOUTTON_DECENDRE_BAS_NON_CLICKE;
            
        }
        Serial.println(_ascenseur.events.surBT_Descendre.etat);
    Serial.println("-----------------------------");

}

void GererAscenseur::fsm(){
    switch (_ascenseur.states )
    {
        case _ascenseur.systemStates::INIT : 
            if (_ascenseur.events.surIR_E0.etat == ASCENSEUR_ETAGE_0_DETECTE)
            {
                _ascenseur.statesPre = _ascenseur.systemStates::INIT;
                _ascenseur.states = _ascenseur.systemStates::ASCENSEUR_EST_EN_ETAGE_0;
                _ascenseur.actions.moteur.etat = ARRERTER_MOTEUR;
                break;
            }
            else if (_ascenseur.events.surIR_E1.etat == ASCENSEUR_ETAGE_1_DETECTE)
            {
                _ascenseur.statesPre = _ascenseur.systemStates::INIT;
                _ascenseur.states = _ascenseur.systemStates::ASCENSEUR_EST_EN_ETAGE_1;
                _ascenseur.actions.moteur.etat = ARRERTER_MOTEUR;
                break;
            }
            
        break;
        
        case _ascenseur.systemStates::ASCENSEUR_EST_EN_ETAGE_0 :
            Serial.println("ASCENSEUR_EST_EN_ETAGE_0");
            if (_ascenseur.events.surBT_Monter.etat == BOUTTON_MONTER_HAUT_CLICKE)
            {
                _ascenseur.actions.moteur.etat = DEMARRER_MOTEUR;
                _ascenseur.actions.moteur.sense = MONTER_ASCENSEUR;
                _ascenseur.statesPre = _ascenseur.systemStates::ASCENSEUR_EST_EN_ETAGE_0;
                _ascenseur.states = _ascenseur.systemStates::ASCENSEUR_EST_MONTANT;
                
            }
        break;
        
        case _ascenseur.systemStates::ASCENSEUR_EST_EN_ETAGE_1 :
            Serial.println("ASCENSEUR_EST_EN_ETAGE_1");
            if (_ascenseur.events.surBT_Descendre.etat == BOUTTON_DECENDRE_BAS_CLICKE)
            {
                _ascenseur.actions.moteur.etat = DEMARRER_MOTEUR;
                _ascenseur.actions.moteur.sense = DESCENDRE_ASCENSEUR;
                _ascenseur.statesPre = _ascenseur.systemStates::ASCENSEUR_EST_EN_ETAGE_1;
                _ascenseur.states = _ascenseur.systemStates::ASCENSEUR_EST_DESENDANT;
            }
        break;

        case _ascenseur.systemStates::ASCENSEUR_EST_MONTANT :
            Serial.println("ASCENSEUR_EST_MONTANT");
            if (_ascenseur.events.surIR_E1.etat == ASCENSEUR_ETAGE_1_DETECTE)
            {
                _ascenseur.actions.moteur.etat = ARRERTER_MOTEUR;
                _ascenseur.statesPre = _ascenseur.systemStates::ASCENSEUR_EST_MONTANT;
                _ascenseur.states = _ascenseur.systemStates::ASCENSEUR_EST_EN_ETAGE_1;
            }
        break;
        
        case _ascenseur.systemStates::ASCENSEUR_EST_DESENDANT :
            Serial.println("ASCENSEUR_EST_DESENDANT");
            if (_ascenseur.events.surIR_E0.etat == ASCENSEUR_ETAGE_0_DETECTE)
            {
                _ascenseur.actions.moteur.etat = ARRERTER_MOTEUR;
                _ascenseur.statesPre = _ascenseur.systemStates::ASCENSEUR_EST_DESENDANT;
                _ascenseur.states = _ascenseur.systemStates::ASCENSEUR_EST_EN_ETAGE_0;
            }
        break;
    }
    Serial.println("-----------------------------");
}

void GererAscenseur::miseAjourActions(){
    if (_ascenseur.actions.moteur.etat == ARRERTER_MOTEUR){
        _driverL298n.arreter();
        Serial.println("arreter moteur");
    }
    else if (_ascenseur.actions.moteur.etat == DEMARRER_MOTEUR && _ascenseur.actions.moteur.sense == DESCENDRE_ASCENSEUR)
    {
        _driverL298n.decendreAscenseur();
        Serial.println("decendre Ascenseur");
    }
    else if (_ascenseur.actions.moteur.etat == DEMARRER_MOTEUR && _ascenseur.actions.moteur.sense == MONTER_ASCENSEUR)
    {
        _driverL298n.monterAscenseur();
        Serial.println("monter Ascenseur");
    } 

    //_viewLcd.updateView();
}


#endif