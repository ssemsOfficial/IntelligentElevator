#ifndef IMMEUBLE_H
#define IMMEUBLE_H

#include <vector>
#include <string>
#include <nlohmann/json.hpp> // JSON library


#include "Ascenseur.h"

//#include"Reservation.h"

using json = nlohmann::json;


class Immeuble
{
public:

    std::string _id;
    std::vector<Ascenseur> _ascenseurs;

    Immeuble()
    {}

    void setId(std::string id){
        _id = id;
    }
    std::string  getId(){
        return _id;
    }
    void ajouAscenseur(Ascenseur ascenseur){
        _ascenseurs.push_back(ascenseur);
    }
    std::vector<Ascenseur> getListeAscenseurs(){
        return _ascenseurs;
    }

};

#endif