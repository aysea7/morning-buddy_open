/*
|  Name: Morning Buddy
|  Description: this alarm clock can send two custom statuses (woke up, did not wake up) to a Telegram channel of your choice via a Telegram bot
|  Parts:
|    -- ESP8266 (Wemos D1 mini or similar)     x1
|    -- Rotary encoder with a clickable button x1
|    -- Wooden box (cigar box)                 x1
|  
|  By Andrii Chubok (aysea7), September 2023
*/

#include <ArduinoJson.h>
#include <EncButton.h>  
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <TimeLib.h>
#include <otadrive_esp.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>

#define PIN_A D6
#define PIN_B D5
#define PIN_BTN D7

volatile int8_t encoderPosition = 0;
volatile bool allChallengesCompleted = false;
unsigned long currentTime = 0;
unsigned long webTime = 0;
unsigned long alarmUNIXbase = 1694041200;
unsigned long nextAlarm = 0;
unsigned long lastAlarm = 0;
unsigned long lastMillis = 0;
unsigned long lastSyncTime = 0;
const uint8_t alarmTime[] = { 17, 45 };   // alarm time {h, m}
const uint16_t timeWindow = 10800;      // time window to solve the tasks within (seconds); default = 10800 (3 hrs)
const unsigned long syncInterval = 900; // Web time synchronisation interval (seconds); default = 900 (15 mins)
const bool winterTime = true;           // an adjustment for the winter time (adds 1 hour to the server time)
const int secondsPerDay = 86400;
const char* ssid = "ssid";
const char* password = "password";

VirtEncoder enc;
Button btn(PIN_BTN);
AsyncWebServer server(80);

void setup() {
  startup();
}

void loop() {
  ota();
  updateCurrentTime();
  checkSync();
  checkAlarm();
  delay(100);
}
