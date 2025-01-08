#ifndef DriverWIFI_H
#define DriverWIFI_H

#include<Arduino.h>

#include<../BSP.h>

#include <WiFi.h>

const char* ssid = "OppoAccessPoint";
const char* wifi_password = "123456789#";

//LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display



class DriverWiFi
{
private:
    
    
public:
    //LiquidCrystal &_lcd;
    WiFiClient& _wifiClient;

    DriverWiFi(WiFiClient& wifiClient)
        : _wifiClient(wifiClient)
    {}
    ~DriverWiFi()
    {}
    
    void init();  

    void connect();
};
void DriverWiFi::init(){
    WiFi.begin(ssid, wifi_password);
}
void DriverWiFi::connect(){
    Serial.println("Connexion au Wi-Fi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connexion en cours...");
    }
    Serial.println("Connecté au Wi-Fi!");
    Serial.print("Adresse IP: ");
    Serial.println(WiFi.localIP());
}

#endif