#include <WiFi.h>
#include "time.h"
#include "Arduino.h"
#include "heltec.h"

const char* ssid     = "...";
const char* password = "...";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 0;

void setup(){
  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, false /*Serial Enable*/);
  Heltec.display->flipScreenVertically();
  Heltec.display->setFont(ArialMT_Plain_24);

  
  Serial.begin(115200);

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void loop(){
  delay(1000);
  printLocalTime();
}

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }

  char timeHour[3];
  strftime(timeHour,3, "%H", &timeinfo);
  char timeMinute[3];
  strftime(timeMinute,3, "%M", &timeinfo);
  char timeSecond[3];
  strftime(timeSecond,3, "%S", &timeinfo);

  char timeDay[3];
  strftime(timeDay,3, "%d", &timeinfo);
  char timeMonth[3];
  strftime(timeMonth,3, "%m", &timeinfo);
  char timeYear[3];
  strftime(timeYear,5, "%Y", &timeinfo);



  Heltec.display->clear();
  Heltec.display->setFont(ArialMT_Plain_24);

  char timeHMS[2+1+2+1+2 +1];
  timeHMS[0] = timeHour[0];
  timeHMS[1] = timeHour[1];
  timeHMS[2] = ':';
  timeHMS[3] = timeMinute[0];
  timeHMS[4] = timeMinute[1];
  timeHMS[5] = ':';
  timeHMS[6] = timeSecond[0];
  timeHMS[7] = timeSecond[1];
  timeHMS[8] = 0;

  Heltec.display->drawString(0, 0, timeHMS);

  char timeDMY[2+1+2+1+4+1 +1];
  timeDMY[0] = timeDay[0];
  timeDMY[1] = timeDay[1];
  timeDMY[2] = '.';
  timeDMY[3] = timeMonth[0];
  timeDMY[4] = timeMonth[1];
  timeDMY[5] = '.';
  timeDMY[6] = timeYear[0];
  timeDMY[7] = timeYear[1];
  timeDMY[8] = timeYear[2];
  timeDMY[9] = timeYear[3];
  timeDMY[10] = '.';
  timeDMY[11] = 0;

  
  Heltec.display->drawString(0, 24, timeDMY);
  

  Heltec.display->display();

  /*
  Serial.println(timeHour);
  Serial.println(timeMinute);
  Serial.println(timeSecond);
  Serial.println(timeDay);
  Serial.println(timeMonth);
  Serial.println(timeYear);
  */
  
}
