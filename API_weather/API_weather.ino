/**
 * BasicHTTPClient.ino
 *
 *  Created on: 24.05.2015
 *
 */

#include <Arduino.h>

#include <WiFi.h>
#include <WiFiMulti.h>

#include <HTTPClient.h>

#include <ArduinoJson.h>

#include "Arduino.h"
#include "heltec.h"


#define ssid "UES"
#define pass "ues_2022"

WiFiMulti wifiMulti;

const int Pin = 0;

int currTemp;
int currPres;
int currHumd;
int currWSpd;
int currWDir;

int nextTemp;
int nextPres;
int nextHumd;
int nextWSpd;
int nextWDir;

int btnState = 0;   
int fl = 0;

void setup() {
    Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, false /*Serial Enable*/);
    Heltec.display->flipScreenVertically();
    Heltec.display->setFont(ArialMT_Plain_24);

    Serial.begin(115200);

    for(uint8_t t = 4; t > 0; t--) {
        Serial.printf("[SETUP] WAIT %d...\n", t);
        Serial.flush();
        delay(1000);
    }

    wifiMulti.addAP(ssid, pass);
    
    if((wifiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;
        
        Serial.print("[HTTP] begin...\n");
        http.begin("https://api.openweathermap.org/data/2.5/forecast?id=524901&appid=54d26bc588d740c3bff55fe2a7a9f975");
        Serial.print("[HTTP] GET...\n");
        
        int httpCode = http.GET();
        if(httpCode > 0) {
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);
            if(httpCode == HTTP_CODE_OK) {
                //String payload = http.getString();

                DynamicJsonDocument doc(16384);
                deserializeJson(doc, http.getString());
                
                currTemp = doc["list"][0]["main"]["temp"];
                currPres = doc["list"][0]["main"]["pressure"];
                currHumd = doc["list"][0]["main"]["humidity"];
                currWSpd = doc["list"][0]["wind"]["speed"];
                currWDir = doc["list"][0]["wind"]["deg"];
                
                nextTemp = doc["list"][8]["main"]["temp"];
                nextPres = doc["list"][8]["main"]["pressure"];
                nextHumd = doc["list"][8]["main"]["humidity"];
                nextWSpd = doc["list"][8]["wind"]["speed"];
                nextWDir = doc["list"][8]["wind"]["deg"];

                /*
                Serial.println(String(currTemp));
                Serial.println(String(currPres));
                Serial.println(String(currHumd));
                Serial.println(String(currWSpd));
                Serial.println(String(currWDir));
                
                Serial.println(String(nextTemp));
                Serial.println(String(nextPres));
                Serial.println(String(nextHumd));
                Serial.println(String(nextWSpd));
                Serial.println(String(nextWDir));
                */                

            }
        }
        else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        
        http.end();
    }

}

void loop() {
    if(!digitalRead(Pin)){
        if(fl){
            fl = 0;
            btnState = !btnState;
        }
    }
    else{
        fl = 1;
    }
    
    Heltec.display->clear();
    Heltec.display->setFont(ArialMT_Plain_10);
  
    if(btnState){
        Heltec.display->clear();
        Heltec.display->setFont(ArialMT_Plain_10);
        Heltec.display->drawString(0, 0, "Current:");
        Heltec.display->drawString(0, 10, "Temperature: " + String(currTemp - 273) + "°C");
        Heltec.display->drawString(0, 20, "Pressure: " + String(currPres) + "hPa");
        Heltec.display->drawString(0, 30, "Humidity: " + String(currHumd) + "%");
        Heltec.display->drawString(0, 40, "Wind Speed: " + String(currWSpd) + "mps");
        Heltec.display->drawString(0, 50, "Wind Direction: " + String(currWDir) + "°");
    }
    else{
        Heltec.display->drawString(0, 0, "Tomorrow:");
        Heltec.display->drawString(0, 10, "Temperature: " + String(nextTemp - 273) + "°C");
        Heltec.display->drawString(0, 20, "Pressure: " + String(nextPres) + "hPa");
        Heltec.display->drawString(0, 30, "Humidity: " + String(nextHumd) + "%");
        Heltec.display->drawString(0, 40, "Wind Speed: " + String(nextWSpd) + "mps");
        Heltec.display->drawString(0, 50, "Wind Direction: " + String(nextWDir) + "°"); 
      
    }
    Heltec.display->display();
    
    delay(100);
}
