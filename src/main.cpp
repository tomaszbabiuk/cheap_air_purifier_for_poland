#include <Arduino.h>
#include <MotorShield.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <GIOSRestApi.h>

ESP8266WiFiMulti WiFiMulti;

#ifndef WIFI_SSID
  #define WIFI_SSID "xxx"
#endif

#ifndef WIFI_PASSWORD
  #define WIFI_PASSWORD "xxx"
#endif

#ifndef GIOS_SENSOR_ID
  #define GIOS_SENSOR_ID 10139
#endif

void setup() {
    MotorShield::init();

    Serial.begin(115200);

    for (uint8_t t = 4; t > 0; t--) {
        Serial.printf("[SETUP] WAIT %d...\n", t);
        Serial.flush();
        delay(1000);
    }

    WiFi.mode(WIFI_STA);
    WiFiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);
}

void loop() {
    if ((WiFiMulti.run() == WL_CONNECTED)) {
        double sensorValue = GIOSRestApi::getSensorData(16784);
        if (sensorValue > 0) {
            Serial.printf("[LOOP] Got sensor data: %.2f\n", sensorValue);
        } else if (sensorValue == GIOS_ERROR_CANNOT_CONNECT) {
            Serial.printf("[LOOP] Cannot connect to GIOS server\n");
        } else if (sensorValue == GIOS_ERROR_NON_200_HTTP_CODE) {
            Serial.printf("[LOOP] GIOS API is reporting problems\n");
        } else if (sensorValue == GIOS_ERROR_PARSING_EXCEPTION) {
            Serial.printf("[LOOP] A problem parsing GIOS SPI response\n");
        }
    }

  delay(60000);
}