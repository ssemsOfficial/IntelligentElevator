#ifndef DRIVERMQTTCLIENT_H
#define DRIVERMQTTCLIENT_H

#include<Arduino.h>

#include<../BSP.h>

#include <string>

#include <PubSubClient.h>
#include <WiFi.h>

// Make sure to update this for your own MQTT Broker!
const char* mqtt_server = "169.254.44.70";
const char* mqtt_topic = "test/topic";
// The client id identifies the ESP8266 device. Think of it a bit like a hostname (Or just a name, like Greg).
const char* clientID = "ascenseur 1";

//LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display



class DriverMqttClient
{
private:
    
    
public:
    //LiquidCrystal &_lcd;
    PubSubClient& _mqttClient; // 1883 is the listener port for the Broker

    DriverMqttClient(PubSubClient& mqttClient)
        : _mqttClient(mqttClient) 
    {}
    ~DriverMqttClient()
    {}

    void connect();
    void publish(std::string dataString);
};

void DriverMqttClient::connect(){
    if (_mqttClient.connect(clientID)) {
        Serial.println("Connected to MQTT Broker!");
    }
    else {
        Serial.println("Connection to MQTT Broker failed...");
    }
}

void DriverMqttClient::publish(std::string dataString){
    if (_mqttClient.publish(mqtt_topic, dataString.c_str())) {
    Serial.println("message sent!");
    }
    // Again, client.publish will return a boolean value depending on whether it succeded or not.
    // If the message failed to send, we will try again, as the connection may have broken.
    else {
        Serial.println("Message failed to send. Reconnecting to MQTT Broker and trying again");
        _mqttClient.connect(clientID);
        delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
        _mqttClient.publish(mqtt_topic, dataString.c_str());
    }
}


#endif