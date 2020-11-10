#include <Arduino.h>
#include <MotorShield.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <GIOSRestApi.h>

ESP8266WiFiMulti WiFiMulti;

#ifndef WIFI_SSID
  #define WIFI_SSID "-"
#endif

#ifndef WIFI_PASSWORD
  #define WIFI_PASSWORD "-"
#endif

#ifndef GIOS_SENSOR_ID
  #define GIOS_SENSOR_ID 16784
#endif

#ifndef REFRESH_INTERVAL_IN_SECONDS
    #define REFRESH_INTERVAL_IN_SECONDS 60*30
#endif

unsigned int _loopCounter; 
double _lastSensorValue;
unsigned int _lastSensorTimeInSeconds;

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
    Serial.printf("[LOOP] Sensor value: %.2f, Loop no: %d, Time: %d [seconds till GMT midnight]\n", _lastSensorValue, _loopCounter, _lastSensorTimeInSeconds);

    if ((WiFiMulti.run() == WL_CONNECTED)) {
        bool refreshNeeded = _loopCounter % REFRESH_INTERVAL_IN_SECONDS == 0;
        if (_lastSensorValue == 0 || refreshNeeded) {
            Serial.printf("[LOOP] Refreshing sensor data\n");
            gios_result sensorValue = GIOSRestApi::getSensorData(GIOS_SENSOR_ID);
            if (sensorValue.error == NoError) {
                Serial.printf("[LOOP] Got sensor data: %.2f\n", sensorValue.sensorData);
                Serial.printf("[LOOP] Time on GIOŚ server is %.2d:%d.2:%.2d GMT\n", sensorValue.time.h, sensorValue.time.m, sensorValue.time.s);
                _lastSensorValue = sensorValue.sensorData;
                _lastSensorTimeInSeconds = sensorValue.time.inSeconds();
            } else if (sensorValue.error == CannotConnect) {
                Serial.printf("[LOOP] Cannot connect to GIOS server\n");
            } else if (sensorValue.error == CannotConnect) {
                Serial.printf("[LOOP] GIOS API is reporting problems\n");
            } else if (sensorValue.error == JsonParsingError) {
                Serial.printf("[LOOP] A problem parsing GIOS SPI response\n");
            }
        }
    } else {
        Serial.printf("[LOOP] Disconnected from WiFi\n");
    }

    delay(1000);

    _loopCounter++;
    _lastSensorTimeInSeconds++;
}