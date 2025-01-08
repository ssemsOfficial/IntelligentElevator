#ifndef  VIEWLCD_H
#define  VIEWLCD_H

#include"../Domaine/Entities/Ascenseur.h"
#include"../Infrastructure/drivers/DriverLCD.h"

class ViewLCD
{
private:
    Ascenseur& _ascenseur;
    DriverLCD& _driverLcd;
public:

    ViewLCD(Ascenseur& ascenseur, DriverLCD& driverLcd)
    : _ascenseur(ascenseur),_driverLcd(_driverLcd) {

    }
    ~ViewLCD(){
        
    }

    void updateView(){
        if(_ascenseur.states = _ascenseur.systemStates::ASCENSEUR_EST_EN_ETAGE_0){
            _driverLcd.setEtage0();
        }
        else if(_ascenseur.states = _ascenseur.systemStates::ASCENSEUR_EST_DESENDANT){
            _driverLcd.setDown();
        }
        else if(_ascenseur.states = _ascenseur.systemStates::ASCENSEUR_EST_EN_ETAGE_1){
            _driverLcd.setEtage1();
        }
        else if(_ascenseur.states = _ascenseur.systemStates::ASCENSEUR_EST_MONTANT){
            _driverLcd.setUp();
        }
    }
};






#endif