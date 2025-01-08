#ifndef ASCENSEURLOADER_H
#define ASCENSEURLOADER_H

//include Framework
#include<Arduino.h>

//include OS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

//BSP
#include"../../../BSP.h"

//libs
#include <vector>
#include <string>
#include <nlohmann/json.hpp> 

#include"../../Entities/Ascenseur.h"
#include"../../Entities/Immeuble.h"
#include"../../Entities/Proprietaire.h"



using json = nlohmann::json;


class AscenseurLoader
{
private:
    /*QueueHandle_t& _queueAscenseurData;*/
    Ascenseur& _ascenseur;
    Immeuble& _immeuble;
    Proprietaire& _proprietaire;

public:
    AscenseurLoader(Ascenseur& ascenseur, Immeuble& immeuble, Proprietaire& proprietaire/*, QueueHandle_t& queueAscenseurData*/)
     : _ascenseur(ascenseur), _immeuble(immeuble), _proprietaire(proprietaire)/*, _queueAscenseurData(queueAscenseurData)*/
    {}
    ~AscenseurLoader()
    {}

    void loadData(json dataRaw);    
};

void AscenseurLoader::loadData(json dataRaw){
    // Check if the JSON data is not empty
    if (!dataRaw.empty()) {
        std::string dataString = dataRaw.dump();
        // Send the serialized data to the queue
        if (true/*xQueueSend(_queueAscenseurData, &dataString, pdMS_TO_TICKS(10)) != pdPASS*/) {
            // Handle queue send failure (e.g., if the queue is full)
            Serial.println("Failed to send data to queue");
        } else {
            Serial.println("Data sent to queue successfully:");
            Serial.println(dataString.c_str());
        }
    } 
    else {
        Serial.println("Empty JSON data received; nothing to load.");
    }
}

#endif