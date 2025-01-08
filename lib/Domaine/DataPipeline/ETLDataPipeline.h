#ifndef ETLDATAPIPELINE
#define ETLDATAPIPELINE


#include <nlohmann/json.hpp> 

#include"../../Infrastructure/drivers/DriverIR.h"
#include"../../Infrastructure/drivers/DriverBoutton.h"
#include"../../Infrastructure/drivers/DriverL298N.h"

#include"../Entities/Ascenseur.h"
#include"../Entities/Immeuble.h"
#include"../Entities/Proprietaire.h"

#include"Transformer/AscenseurTransformer.h"

#include"Loader/AscenseurLoader.h"

using json = nlohmann::json;

class ETLDataPipeline
{
private:
    Ascenseur& _ascenseur;
    Immeuble& _immeuble;
    Proprietaire& _proprietaire;
    
    DriverIR& _driverIR_E0;
    DriverIR& _driverIR_E1; 
    DriverBoutton& _driverBouttonE0;
    DriverBoutton& _driverBouttonE1;

    /*QueueHandle_t& _queueAscenseurData;*/

    AscenseurTransformer _ascenseurTransformer;
    AscenseurLoader _ascenseurLoader;


public:
    ETLDataPipeline(DriverIR& driverIR_E0, DriverIR& driverIR_E1, DriverBoutton& driverBouttonE0, DriverBoutton& driverBouttonE1,   Ascenseur& ascenseur, Immeuble& immeuble, Proprietaire& proprietaire/*, QueueHandle_t& queueAscenseurData*/)
     :_driverIR_E0(driverIR_E0), _driverIR_E1(driverIR_E1), _driverBouttonE0(driverBouttonE0), _driverBouttonE1(driverBouttonE1),   _ascenseur(ascenseur), _immeuble(immeuble),  _proprietaire(proprietaire), /*_queueAscenseurData(queueAscenseurData),*/
      _ascenseurTransformer(ascenseur, immeuble, proprietaire),  _ascenseurLoader(ascenseur, immeuble, proprietaire/*, queueAscenseurData*/)
    {}
    ~ETLDataPipeline()
    {}

    void extractSensorsData();
    json transformData();
    void loadData(json rawData);
};

void ETLDataPipeline::extractSensorsData(){
    _driverIR_E0.estPositionDetecte();
    _driverIR_E1.estPositionDetecte();
    _driverBouttonE0.estclicke();
    _driverBouttonE1.estclicke();
}

json ETLDataPipeline::transformData(){
    return _ascenseurTransformer.transformData();
}

void ETLDataPipeline::loadData(json rawData){
    _ascenseurLoader.loadData(rawData);
}

#endif