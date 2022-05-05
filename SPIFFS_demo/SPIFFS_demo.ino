#include "SPIFFS.h"
void setup(){
  Serial.begin(115200);
  if(!SPIFFS.begin(true)){ 
    Serial.println("Initialization error: SPIFFS");
    return;
  }
  File file = SPIFFS.open("/example.txt");
  if(!file){
    Serial.println("File open error: SPIFFS");
    return;
  }
  Serial.println("File contnet:");
  while(file.available())
  Serial.write(file.read());
  file.close();
}

void loop(){}
