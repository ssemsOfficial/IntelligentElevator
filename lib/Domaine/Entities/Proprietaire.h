#ifndef PROPRIETAIRE_H
#define PROPRIETAIRE_H

#include <vector>
#include <string>
#include <nlohmann/json.hpp> // JSON library


#include "Ascenseur.h"
#include "Immeuble.h"

//#include"Reservation.h"

using json = nlohmann::json;


class Proprietaire
{
public:

    std::string _id;
    std::vector<Immeuble> _immeubles;

    Proprietaire()
    {}

    void setId(std::string id){
        _id = id;
    }
    std::string  getId(){
        return _id;
    }
    void ajouImmeuble(Immeuble Immeuble){
        _immeubles.push_back(Immeuble);
    }
    std::vector<Immeuble> getListeImmeubles(){
        return _immeubles;
    }

};

#endif