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
    #define REFRESH_INTERVAL_IN_SECONDS 1800
#endif

#define PM10_VALUE_FOR_MAX_SPEED 300.0
#define PM10_VALUE_FOR_MIN_SPEED 50.0

unsigned int _loopCounter; 
gios_result _lastSensorValue;

enum working_mode {
    P100,
    P75,
    P50,
    P25,
    PMin,
    Auto,
    AutoQuiet,
    Off
} _mode;

working_mode _workingSchedule[24][12] {
    // 0:00 - 1:00 GMT, 1:00 - 2:00 GMT + 1
    { Off, Off, Off, Off, Off, Off, Off, Off, Off, Off, Off, Off },  
    
    // 1:00 - 2:00 GMT, 2:00 - 3:00 GMT + 1
    { Off, Off, Off, Off, Off, Off, Off, Off, Off, Off, Off, Off },  

    // 2:00 - 3:00 GMT, 3:00 - 4:00 GMT + 1
    { Off, Off, Off, Off, Off, Off, Off, Off, Off, Off, Off, Off },

    // 3:00 - 4:00 GMT, 4:00 - 5:00 GMT + 1  
    { Off, Off, Off, Off, Off, Off, Off, Off, Off, Off, Off, Off },
    
    // 4:00 - 5:00 GMT, 5:00 - 6:00 GMT + 1
    { Off, Off, Off, Off, Off, Off, Off, Off, Off, Off, Off, Off },  
    
    // 5:00 - 6:00 GMT, 6:00 - 7:00 GMT + 1
    { Off, Off, Off, Off, Off, Off, Off, Off, Off, Off, Off, Off },  
    
    // 6:00 - 7:00 GMT, 7:00 - 8:00 GMT + 1
    { Off, Off, Off, Off, Off, Off, PMin, PMin, PMin, PMin, P50, P100 },  
    
    // 7:00 - 8:00 GMT, 7:00 - 9:00 GMT + 1
    { P50, PMin, Auto, Auto, Auto, Auto, Auto, Auto, Auto, Auto, Auto, Auto },  
    
    // 8:00 - 9:00 GMT, 9:00 - 10:00 GMT + 1
    { Auto, Auto, Auto, Auto, Auto, Auto, Auto, Auto, Auto, Auto, Auto, Auto },  
    
    // 9:00 - 10:00 GMT, 10:00 - 11:00 GMT + 1
    { AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet},  
    
    // 10:00 - 11:00 GMT, 11:00 - 12:00 GMT + 1
    { AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet},  
    
    // 11:00 - 12:00 GMT, 12:00 - 13:00 GMT + 1
    { AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet},  
    
    // 12:00 - 13:00 GMT, 13:00 - 14:00 GMT + 1
    { AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet},  
    
    // 13:00 - 14:00 GMT, 14:00 - 15:00 GMT + 1
    { AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet},  

    // 14:00 - 15:00 GMT, 15:00 - 16:00 GMT + 1
    { AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet, AutoQuiet},  
    
    // 15:00 - 16:00 GMT, 16:00 - 17:00 GMT + 1
    { Auto, Auto, Auto, Auto, Auto, Auto, Auto, Auto, Auto, Auto, Auto, Auto }, 
    
    // 16:00 - 17:00 GMT, 17:00 - 18:00 GMT + 1
    { Auto, Auto, Auto, Auto, Auto, Auto, Auto, Auto, Auto, Auto, Auto, Auto }, 
    
    // 17:00 - 18:00 GMT, 18:00 - 19:00 GMT + 1
    { Auto, Auto, Auto, Auto, Auto, Auto, Auto, Auto, Auto, Auto, Auto, Auto }, 
    
    // 18:00 - 19:00 GMT, 19:00 - 20:00 GMT + 1
    { Auto, Auto, Auto, Auto, Auto, Auto, Auto, Auto, Auto, Auto, Auto, Auto }, 
    
    // 19:00 - 20:00 GMT, 20:00 - 21:00 GMT + 1
    { Auto, Auto, Auto, Auto, Auto, Auto, Auto, Auto, Auto, Auto, Auto, Auto }, 
    
    // 20:00 - 21:00 GMT, 21:00 - 22:00 GMT + 1
    { Auto, Auto, P25, P50, P50, P25, Off, Off, Off, Off, Off, Off },  
    
    // 21:00 - 22:00 GMT, 22:00 - 23:00 GMT + 1
    { Off, Off, Off, Off, Off, Off, Off, Off, Off, Off, Off, Off },  
    
    // 22:00 - 23:00 GMT, 23:00 - 00:00 GMT + 1
    { Off, Off, Off, Off, Off, Off, Off, Off, Off, Off, Off, Off },  
    
    // 23:00 - 0:00 GMT, 0:00 - 1:00 GMT + 1
    { Off, Off, Off, Off, Off, Off, Off, Off, Off, Off, Off, Off }   
};

void setup() {
    MotorShield::init();

    Serial.begin(115200);

    WiFi.mode(WIFI_STA);
    WiFiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);
}

working_mode pickWorkingMode(gios_result sensorValue) {
    if (sensorValue.sensorData == 0 || sensorValue.time.invalidFormat) {
        return PMin;
    }

    return _workingSchedule[sensorValue.time.h][sensorValue.time.m / 5];
}

int adjustFanSpeedToPollution(double pm10) {
    if (pm10 > PM10_VALUE_FOR_MIN_SPEED) {
        double valueCorrected = pm10 > PM10_VALUE_FOR_MAX_SPEED ? PM10_VALUE_FOR_MAX_SPEED : pm10;
        return 1023.0/PM10_VALUE_FOR_MAX_SPEED*valueCorrected;
    } else {
        return 0;
    }
}

void operate(gios_result sensorValue, int loopCounter) {
    working_mode mode = pickWorkingMode(sensorValue);
    switch (mode) {
        case PMin: 

            if (loopCounter % 4 == 0) {
                MotorShield::driveMotorA(0, 1023);
                delay(350);
            }

            MotorShield::driveMotorA(0, 0);
            break;
        case P25:
            MotorShield::driveMotorA(0, 255);
            break;
        case P50:
            MotorShield::driveMotorA(0, 511);
            break;
        case P75:
            MotorShield::driveMotorA(0, 767);
            break;
        case P100:
            MotorShield::driveMotorA(0, 1023);
            break;
        case Auto:
            MotorShield::driveMotorA(0, adjustFanSpeedToPollution(sensorValue.sensorData));
            break;
        case AutoQuiet:
            MotorShield::driveMotorA(0, adjustFanSpeedToPollution(sensorValue.sensorData)/2);
            break;
        default:
            MotorShield::driveMotorA(0, 0);
    }
}

void loop() {
    Serial.printf("[LOOP] Sensor value: %.2f, Loop no: %d, Time: %d [seconds from GMT midnight]\n", 
        _lastSensorValue.sensorData, _loopCounter, _lastSensorValue.time.inSeconds());

    if ((WiFiMulti.run() == WL_CONNECTED)) {
        bool refreshNeeded = _loopCounter % REFRESH_INTERVAL_IN_SECONDS == 0;
        if (_lastSensorValue.sensorData == 0 || refreshNeeded) {
            Serial.printf("[LOOP] Refreshing sensor data\n");

            if (refreshNeeded) {
                Serial.println("Refresh is needed\n");
            }

            gios_result sensorValue = GIOSRestApi::getSensorData(GIOS_SENSOR_ID);
            if (sensorValue.error == NoError) {
                Serial.printf("[LOOP] Got sensor data: %.2f\n", sensorValue.sensorData);
                Serial.printf("[LOOP] Time on GIOŚ server is %d:%d:%d GMT\n", sensorValue.time.h, sensorValue.time.m, sensorValue.time.s);
                _lastSensorValue = sensorValue;
            } else if (sensorValue.error == CannotConnect) {
                Serial.printf("[LOOP] Cannot connect to GIOS server\n");
            } else if (sensorValue.error == Non200HttpCode) {
                Serial.printf("[LOOP] GIOS API is reporting problems\n");
            } else if (sensorValue.error == JsonParsingError) {
                Serial.printf("[LOOP] A problem parsing GIOS SPI response\n");
            }
        }
    } else {
        Serial.printf("[LOOP] Disconnected from WiFi\n");
    }

    _loopCounter++;
    _lastSensorValue.time.increment();

    operate(_lastSensorValue, _loopCounter);
    delay(1000);
}