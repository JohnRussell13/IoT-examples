#include <WiFi.h>
#include "time.h"

#define uS_TO_S_FACTOR 1000000  //Conversion factor for micro seconds to seconds

const char* ssid     = "...";
const char* password = "...";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 0;

int timeToSleep;

void setup(){
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
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
  }

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);

  //Print the wakeup reason for ESP32
  print_wakeup_reason();

  timeToSleep = getTimeInterval();
  
  //Set timer to ? seconds
  esp_sleep_enable_timer_wakeup(uS_TO_S_FACTOR * timeToSleep);
  
  //Go to sleep now
  esp_deep_sleep_start();
}

void loop(){}

//Function that prints the reason by which ESP32 has been awaken from sleep
void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  switch(wakeup_reason)
  {
    case 2  : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case 1  : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case 4  : Serial.println("Wakeup caused by timer"); break;
    case 3  : Serial.println("Wakeup caused by touchpad"); break;
    case 5  : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.println("Wakeup was not caused by deep sleep"); break;
  }
}

int getTimeInterval(){
  while(!Serial.available()){
    delay(500);
  }
  String inTime = Serial.readString();
  
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return -1;
  }

  char timeHour[3];
  strftime(timeHour,3, "%H", &timeinfo);
  char timeMinute[3];
  strftime(timeMinute,3, "%M", &timeinfo);
  char timeSecond[3];
  strftime(timeSecond,3, "%S", &timeinfo);
  

  int inHour = (inTime[0] - '0') *10 + (inTime[1] - '0');
  int nowHour = (timeHour[0] - '0') *10 + (timeHour[1] - '0');
  
  int inMinute = (inTime[3] - '0') *10 + (inTime[4] - '0');
  int nowMinute = (timeMinute[0] - '0') *10 + (timeMinute[1] - '0');
  
  int inSecond = (inTime[6] - '0') *10 + (inTime[7] - '0');
  int nowSecond = (timeSecond[0] - '0') *10 + (timeSecond[1] - '0');

  /*
  Serial.println(String(inHour));
  Serial.println(String(nowHour));
  Serial.println(String(inMinute));
  Serial.println(String(nowMinute));
  Serial.println(String(inSecond));
  Serial.println(String(nowSecond));
  Serial.println(String((inHour - nowHour)*3600 + (inMinute - nowMinute)*60 + (inSecond - nowSecond)));
  */

  return (inHour - nowHour)*3600 + (inMinute - nowMinute)*60 + (inSecond - nowSecond);

  
}
