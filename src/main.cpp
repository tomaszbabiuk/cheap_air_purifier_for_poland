#include <Arduino.h>
#include <MotorShield.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <GIOSRestApi.h>

ESP8266WiFiMulti WiFiMulti;

#ifndef WIFI_SSID
  #define WIFI_SSID "---"
#endif

#ifndef WIFI_PASSWORD
  #define WIFI_PASSWORD "---"
#endif

#ifndef GIOS_SENSOR_ID
  #define GIOS_SENSOR_ID 16784
#endif

#ifndef REFRESH_INTERVAL_IN_SECONDS
    #define REFRESH_INTERVAL_IN_SECONDS 60
#endif

uint32_t _loopCounter; 
double _lastSensorValue;

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
    Serial.printf("[LOOP] Sensor value: %.2f, Loop no: %d\n", _lastSensorValue, _loopCounter);

    if ((WiFiMulti.run() == WL_CONNECTED)) {
        bool refreshNeeded = _loopCounter % REFRESH_INTERVAL_IN_SECONDS == 0;
        if (_lastSensorValue == 0 || refreshNeeded) {
            Serial.printf("[LOOP] Refreshing sensor data\n");
            double sensorValue = GIOSRestApi::getSensorData(GIOS_SENSOR_ID);
            if (sensorValue > 0) {
                Serial.printf("[LOOP] Got sensor data: %.2f\n", sensorValue);
                _lastSensorValue = sensorValue;
            } else if (sensorValue == GIOS_ERROR_CANNOT_CONNECT) {
                Serial.printf("[LOOP] Cannot connect to GIOS server\n");
            } else if (sensorValue == GIOS_ERROR_NON_200_HTTP_CODE) {
                Serial.printf("[LOOP] GIOS API is reporting problems\n");
            } else if (sensorValue == GIOS_ERROR_PARSING_EXCEPTION) {
                Serial.printf("[LOOP] A problem parsing GIOS SPI response\n");
            }
        }
    } else {
        Serial.printf("[LOOP] Disconnected from WiFi\n");
    }

    delay(1000);

    _loopCounter++;
}