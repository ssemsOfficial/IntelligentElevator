#include <Arduino.h>

//BSP
#include<../lib/BSP.h>

//libraries
 //extern
#include <Wire.h>
#include <WiFi.h>
#include <nlohmann/json.hpp> // Include the JSON library
#include <ArduinoJson.h>
#include <stdio.h>
#include <Base64.h> // Include Base64 library for encoding (install via Arduino Library Manager if needed)
#include <string>
#include <mbedtls/ecp.h>
#include <mbedtls/pem.h>
#include <mbedtls/error.h>
#include <mbedtls/ecdsa.h>
#include <mbedtls/ecdsa.h>
#include <mbedtls/pk.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
//#include <Wire.h>
//#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal_I2C.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"


 //components
#include<../lib/Infrastructure/components/Boutton.h>
#include<../lib/Infrastructure/components/IRSensor.h>
#include<../lib/Infrastructure/components/L298N.h>
#include<../lib/Infrastructure/components/Relay.h>
 //drivers
#include<../lib/Infrastructure/drivers/DriverBoutton.h>
#include<../lib/Infrastructure/drivers/DriverIR.h>
#include<../lib/Infrastructure/drivers/DriverL298N.h>
#include<../lib/Infrastructure/drivers/DriverWiFi.h>
#include<../lib/Infrastructure/drivers/DriverMqttClient.h>
//#include<../lib/Infrastructure/drivers/DriverLCD.h>
 //Entities
#include<../lib/Domaine/Entities/Ascenseur.h>
#include<../lib/Domaine/Entities/Immeuble.h>
#include<../lib/Domaine/Entities/Proprietaire.h>
 //systems
#include<../lib/Business/Systems/GererAscenseur.h>
 //Data pipeline
#include<../lib/Domaine/DataPipeline/ETLDataPipeline.h>
 //presentation 
//#include<../lib/Presentation/ViewLCD.h> 


//variables
 //inputs
Boutton boutton_Monter(pinBouttonMonter);
DriverBoutton driverBoutton_Monter(boutton_Monter);
Boutton boutton_Descendre(pinBouttonDescendre);
DriverBoutton driverBoutton_Descendre(boutton_Descendre);
IRSensor irSensor_E0(pinIRSensorEtage0);
DriverIR driverIR_E0(irSensor_E0);
IRSensor irSensor_E1(pinIRSensorEtage1);
DriverIR driverIR_E1(irSensor_E1);
WiFiClient wifiClient;
DriverWiFi driverWifi(wifiClient);
PubSubClient mqttClient(mqtt_server, 1883, wifiClient);
DriverMqttClient driverMqttClient(mqttClient); 
LiquidCrystal_I2C lcd(0x27, 16, 2);
//DriverLCD driverLCD(lcd);

 //output
L298N l298n(pinMotor1Pin1, pinMotor1Pin2);
DriverL298N driverL298N(l298n);
 //memoire
  //systeme
Ascenseur ascenseur;
Immeuble immeuble;
Proprietaire proprietaire;
 //presentation
//ViewLCD viewLCD(ascenseur, driverLCD); 


//create queue
QueueHandle_t queueAscenseurData = xQueueCreate(5, 10*sizeof(String));

//create semaphore
SemaphoreHandle_t semaphore = xSemaphoreCreateBinary();

GererAscenseur  gererAscenseur(ascenseur, driverL298N, driverIR_E0, driverIR_E1, driverBoutton_Monter, driverBoutton_Descendre/*, viewLCD*/);

 //data
ETLDataPipeline dataPipeline(driverIR_E0, driverIR_E1, driverBoutton_Monter, driverBoutton_Descendre, ascenseur, immeuble, proprietaire/*, queueAscenseurData*/);
 


////////////////////////// MQTT
#define I2C_SDA 21
#define I2C_SCL 22

//////////////////////////////////////////////////////
/*
//////////////////////////cryptage////////////////////////
// Embedded private and public keys (from Raspberry Pi)
const char* private_key_pem = R"(
-----BEGIN EC PRIVATE KEY-----
MHcCAQEEIBuaAJeQbf0yi1pLj6Zbmw4k064n8ww8Xh3dLNv1Smq4oAoGCCqGSM49
AwEHoUQDQgAErqCwAu7i80nxKMfjIKUAWML841sztJD20K+U6VnlpsTZqtzeYcx3
tPV4cm3CigkZ/Wl4PorITKzoQqwL2f0gmA==
-----END EC PRIVATE KEY-----
)";

const char* public_key_pem = R"(
-----BEGIN PUBLIC KEY-----
MFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAErqCwAu7i80nxKMfjIKUAWML841sz
tJD20K+U6VnlpsTZqtzeYcx3tPV4cm3CigkZ/Wl4PorITKzoQqwL2f0gmA==
-----END PUBLIC KEY-----
)";

    mbedtls_ecp_keypair keypair;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_ecp_point ecp_point;


    mbedtls_pk_context prik;
    mbedtls_pk_context pubk;

void load_private_key(const char *pem, mbedtls_ecdsa_context *ctx, mbedtls_pk_context &pk) {
    int ret = mbedtls_pk_parse_key(&pk, (const unsigned char *)pem, strlen(pem) + 1, NULL, 0);
    if (ret != 0) {
        char error_buf[100];
        mbedtls_strerror(ret, error_buf, 100);
        printf("Error loading private key: %s\n", error_buf);
    } else {
        mbedtls_ecdsa_context *ecdsa_ctx = mbedtls_pk_ec(pk);
        memcpy(ctx, ecdsa_ctx, sizeof(mbedtls_ecdsa_context));
    }
}

void load_public_key(const char *pem, mbedtls_ecp_point *pub_key, mbedtls_pk_context &pk) {
    int ret = mbedtls_pk_parse_public_key(&pk, (const unsigned char *)pem, strlen(pem) + 1);
    if (ret != 0) {
        char error_buf[100];
        mbedtls_strerror(ret, error_buf, 100);
        printf("Error loading public key: %s\n", error_buf);
    } else {
        mbedtls_ecp_keypair *keypair = mbedtls_pk_ec(pk);
        memcpy(pub_key, &keypair->Q, sizeof(mbedtls_ecp_point));
    }
}


void encrypt_data(const char *data, mbedtls_ecp_point *pub_key, mbedtls_ecdsa_context *private_key) {
    // Perform ECC encryption (e.g., shared secret generation, AES encryption)
    printf("Encrypting data...\n");
}


/////////////////////////////////////////////////////////
*/


void gererAscenseurTask(void *parameter){
        String dataAscenseur;

    for (;;)
    {
        if (xSemaphoreTake(semaphore, portMAX_DELAY) == pdTRUE) {
            //Extract
            dataPipeline.extractSensorsData();

            gererAscenseur.miseAjourEvenement();
            gererAscenseur.fsm();
            gererAscenseur.miseAjourActions();
            
            auto dataJson = dataPipeline.transformData();
            /*
            dataPipeline.loadData(dataJson);

            if (xQueueReceive(queueAscenseurData, &dataAscenseur, portMAX_DELAY))
            {
                driverMqttClient.publish(dataAscenseur.c_str()); 
            }
            vTaskDelay(500 / portTICK_PERIOD_MS); 

            xSemaphoreGive(semaphore);
            */
           if (!dataJson.empty()) {
                std::string dataString = dataJson.dump();
                // Send the serialized data to the queue
                /*
                if (xQueueSend(queueAscenseurData, &dataString, pdMS_TO_TICKS(10)) != pdPASS) {
                    // Handle queue send failure (e.g., if the queue is full)
                    Serial.println("Failed to send data to queue");
                } else {
                    Serial.println("Data sent to queue successfully:");
                    Serial.println(dataString.c_str());
                }
                if (xQueueReceive(queueAscenseurData, &dataAscenseur, portMAX_DELAY))
                {
                    try
                    {
                        driverMqttClient.publish(dataAscenseur.c_str()); 
                    }
                    catch(const std::exception& e)
                    {
                        Serial.println("error in mqtt");
                    }
                }
                */
               try
                {
                    driverMqttClient.publish(dataString.c_str()); 
                }
                catch(const std::exception& e)
                {
                    Serial.println("error in mqtt");
                }
            } 
            else {
                Serial.println("Empty JSON data received; nothing to load.");
            }
            xSemaphoreGive(semaphore);
        }
        
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

/*
void senderTask(void *parameter){
    
    String dataAscenseur="empty";
    bool interventionMain;
    
    for (;;)
    {
        if (xSemaphoreTake(semaphore, portMAX_DELAY) == pdTRUE) {
            //Serialize JSON to a string
            if (xQueueReceive(queueAscenseurData, &dataAscenseur, portMAX_DELAY))
            {
                driverMqttClient.publish(dataAscenseur.c_str()); 
            }
            vTaskDelay(500 / portTICK_PERIOD_MS);  

            xSemaphoreGive(semaphore);  
        }    
    }
}
*/

void setup(){
  //init material
  Serial.begin(115200);
  boutton_Monter.init();
  boutton_Descendre.init();
  irSensor_E0.init();
  irSensor_E1.init();
  l298n.init();
  WiFi.begin(ssid, wifi_password);
   driverWifi.connect();
  //lcd.init
  //driverLCD.init();
   driverMqttClient.connect();
    
    lcd.init(); // initialize the lcd

    //Create Semaphore
    if (semaphore == NULL) {
        Serial.println("Failed to create semaphore!");
        while (1) {
            delay(1000); // Halt execution as semaphore creation failed
        }
    }
    // Give semaphore initially
    xSemaphoreGive(semaphore);
    
    xTaskCreate(gererAscenseurTask, "Gerer Ascenseur", 2048, NULL, 1, NULL);
    //xTaskCreate(senderTask, "Sender Task", 2048, NULL, 1, NULL);




/*
  ///////////////cryptage/////////////////////
  // Initialize ECC contexts
    mbedtls_pk_init(&prik);
    mbedtls_pk_init(&pubk);
    mbedtls_ecp_keypair_init(&keypair);

    // Load keys
    load_private_key(private_key_pem, &keypair, prik);
    load_public_key(public_key_pem, &ecp_point, pubk);
    

    

    // Free resources
    //mbedtls_pk_free(&prik);
    //mbedtls_pk_free(&pubk);
    //mbedtls_ecp_keypair_free(&keypair);
    //mbedtls_ecp_point_free(&ecp_point);
*/
}

void loop() {
/*
    lcd.clear();              // clear display
    lcd.setCursor(6, 0);      // move cursor to   (0, 0)
    lcd.print("IOT");       // print message at (0, 0)
    lcd.setCursor(5, 1);      // move cursor to   (2, 1)
    lcd.print("EC Dept"); // print message at (2, 1)
    delay(2000);                  // display the above for two seconds

    lcd.clear();                  // clear display
    lcd.setCursor(2, 0);          // move cursor to   (3, 0)
    lcd.print("LCD I2C MODE");        // print message at (3, 0)
    lcd.setCursor(6, 1);          // move cursor to   (0, 1)
    lcd.print("16x2"); // print message at (0, 1)
    delay(2000);                      // display the above for two seconds

*/
/*
    if (Serial.available() > 0) {
        // Read input from serial
        String input = Serial.readStringUntil('\n');
        Serial.println("Input: " + input);

        // Convert input to unsigned char array for processing
        size_t input_len = input.length();
        unsigned char plaintext[input_len];
        memcpy(plaintext, input.c_str(), input_len);

        // Buffers for encryption and decryption
        unsigned char encrypted_data[256];
        unsigned char decrypted_data[256];
        size_t encrypted_len = 0;
        size_t decrypted_len = 0;

        // Encrypt the message using the public key
        Serial.println("Encrypting message...");
        int encrypt_ret = mbedtls_ecp_point_write_binary(
            &keypair.grp, &keypair.Q, MBEDTLS_ECP_PF_UNCOMPRESSED,
            &encrypted_len, encrypted_data, sizeof(encrypted_data));

        if (encrypt_ret != 0) {
            char error_buf[100];
            mbedtls_strerror(encrypt_ret, error_buf, 100);
            Serial.printf("Encryption failed: %s\n", error_buf);
            return;
        }

        Serial.print("Encrypted data: ");
        for (size_t i = 0; i < encrypted_len; i++) {
            Serial.printf("%02X ", encrypted_data[i]);
        }
        Serial.println();
        

        
        // Decrypt the message using the private key
        Serial.println("Decrypting message...");
        int decrypt_ret = mbedtls_ecp_point_read_binary(
            &keypair.grp, &keypair.Q, encrypted_data, encrypted_len);

        if (decrypt_ret != 0) {
            char error_buf[100];
            mbedtls_strerror(decrypt_ret, error_buf, 100);
            Serial.printf("Decryption failed: %s\n", error_buf);
            return;
        }

        memcpy(decrypted_data, encrypted_data, encrypted_len);
        decrypted_len = encrypted_len;

        // Null-terminate and print the decrypted message
        decrypted_data[decrypted_len] = '\0';
        String decryptedString = String((char*)decrypted_data);

        Serial.println(decryptedString.c_str()); // Cast to char* before creating a String
        // Free resources
        //mbedtls_ecp_keypair_free(&keypair);
        //mbedtls_ecp_point_free(&keypair.Q);
    }


*/
  
  
  //delay(500);

}

