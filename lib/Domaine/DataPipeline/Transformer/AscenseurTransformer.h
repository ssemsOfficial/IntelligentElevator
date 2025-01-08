#ifndef ASCENSEURTRANSFORMER_H
#define ASCENSEURTRANSFORMER_H

#include<Arduino.h>

#include <vector>
#include <string>
#include <nlohmann/json.hpp> 

#include"../../../BSP.h"

#include"../../Entities/Ascenseur.h"
#include"../../Entities/Immeuble.h"
#include"../../Entities/Proprietaire.h"

using json = nlohmann::json;


class AscenseurTransformer
{
private:
    Ascenseur& _ascenseur;
    Immeuble& _immeuble;
    Proprietaire& _proprietaire;

public:
    AscenseurTransformer(Ascenseur& ascenseur, Immeuble& immeuble, Proprietaire& proprietaire)
     : _ascenseur(ascenseur), _immeuble(immeuble), _proprietaire(proprietaire)
    {}
    ~AscenseurTransformer()
    {}



    json toJson(std::string etat) const {
          json ascenseursJsonArray = json::array();
          _ascenseur.setId("ascenseur_1");
          ascenseursJsonArray.push_back(json{
               {"id", _ascenseur.getId()},
               {"etat", etat}
          });

          json immeublesJsonArray = json::array();
          _immeuble.setId("immeuble_1");
          immeublesJsonArray.push_back({
               {"id", _immeuble.getId()},
               {"ascenseurs", ascenseursJsonArray}
          });

        _proprietaire.setId("houssem");  
        return json{
            
            {"id", _proprietaire.getId()},
            {"immeubles", immeublesJsonArray}
        };
    }

    json transformData();
    
};

json AscenseurTransformer::transformData(){
   //if data repeated 
   if(_ascenseur.states == _ascenseur.systemStates::ASCENSEUR_EST_EN_ETAGE_0 && _ascenseur.states != _ascenseur.statesPre)
   {
        Serial.println("ASCENSEUR states : ");
        Serial.println(_ascenseur.states);
        Serial.println("ASCENSEUR_EST_EN_ETAGE_0");
        return toJson("ASCENSEUR EST EN ETAGE 0");
   }
   else if(_ascenseur.states == _ascenseur.systemStates::ASCENSEUR_EST_MONTANT && _ascenseur.states != _ascenseur.statesPre)
   {
        Serial.println("ASCENSEUR_EST_MONTANT");
        return toJson("ASCENSEUR EST MONTANT");
   }
   else if(_ascenseur.states == _ascenseur.systemStates::ASCENSEUR_EST_EN_ETAGE_1 && _ascenseur.states != _ascenseur.statesPre)
   {
        Serial.println("ASCENSEUR_EST_EN_ETAGE_1");
        return toJson("ASCENSEUR EST EN ETAGE 1");
   }
   else if(_ascenseur.states == _ascenseur.systemStates::ASCENSEUR_EST_DESENDANT && _ascenseur.states != _ascenseur.statesPre)
   {
        Serial.println("ASCENSEUR_EST_DESCENDANT");
        return toJson("ASCENSEUR EST DESCENDANT");
   }
   Serial.println("No state change detected.");
   return json::object();
}



#endif