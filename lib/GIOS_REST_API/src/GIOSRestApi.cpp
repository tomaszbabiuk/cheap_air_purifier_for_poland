/*
  GIOSRestApi.h - Library for getting sensor values from GIOS REST API. See details here: http://powietrze.gios.gov.pl/pjp/content/api
  Author - Tomasz Babiuk
*/

#include <Arduino.h>
#include "GIOSRestAPI.h"
#include "GIOSTimeParser.h"
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

static WiFiClient _client;
static HTTPClient _http;
static DynamicJsonBuffer _jsonBuffer;

#define DATE_HEADER "date"
const char * headerkeys[] = { DATE_HEADER };

gios_result GIOSRestApi::getSensorData(int sensorId) {
    gios_result result;
    
    if (_http.begin(_client, "http://api.gios.gov.pl/pjp-api/rest/data/getData/" + String(sensorId))) {
        _http.collectHeaders(headerkeys, 1);
        int httpCode = _http.GET();

        if (httpCode == 200) {
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
                JsonObject& root = _jsonBuffer.parseObject(_http.getString());
                String date = _http.header(DATE_HEADER);
                result.time = GIOSTimeParser::fromGMTString(date.c_str(), date.length());

                for (int i=0; i<10; i++) {
                    String valueAsString = root[(String)"values"][i]["value"];
                    double valueAsDouble = atof(valueAsString.c_str());
                    if (valueAsDouble != 0) {
                        result.sensorData = valueAsDouble;
                        return result;
                    }
                }

                result.error = JsonParsingError;
                return result;
            }
        } else {
            result.error = Non200HttpCode;
            return result;
        }

        _http.end();
    }

    result.error = CannotConnect;
    return result;
}


/* 
    Example paylod from GIOŚ server:

    {"key":"PM10","values":[{"date":"2020-11-09 21:00:00","value":null},{"date":"2020-11-09 20:00:00","value":null},{"date":"2020-11-09 19:00:00","value":40.6805},{"date":"2020-11-09 18:00:00","value":57.2251},{"date":"2020-11-09 17:00:00","value":51.5279},{"date":"2020-11-09 16:00:00","value":44.709},{"date":"2020-11-09 15:00:00","value":52.0372},{"date":"2020-11-09 14:00:00","value":58.7376},{"date":"2020-11-09 13:00:00","value":40.7967},{"date":"2020-11-09 12:00:00","value":42.5111},{"date":"2020-11-09 11:00:00","value":31.9872},{"date":"2020-11-09 10:00:00","value":33.9012},{"date":"2020-11-09 09:00:00","value":40.755},{"date":"2020-11-09 08:00:00","value":35.0845},{"date":"2020-11-09 07:00:00","value":30.321},{"date":"2020-11-09 06:00:00","value":50.9288},{"date":"2020-11-09 05:00:00","value":59.5386},{"date":"2020-11-09 04:00:00","value":58.0884},{"date":"2020-11-09 03:00:00","value":70.0565},{"date":"2020-11-09 02:00:00","value":89.1899},{"date":"2020-11-09 01:00:00","value":93.1226},{"date":"2020-11-09 00:00:00","value":92.3493},{"date":"2020-11-08 23:00:00","value":89.7364},{"date":"2020-11-08 22:00:00","value":76.1283},{"date":"2020-11-08 21:00:00","value":null},{"date":"2020-11-08 20:00:00","value":null},{"date":"2020-11-08 19:00:00","value":62.2775},{"date":"2020-11-08 18:00:00","value":49.1586},{"date":"2020-11-08 17:00:00","value":39.1903},{"date":"2020-11-08 16:00:00","value":42.9141},{"date":"2020-11-08 15:00:00","value":19.7036},{"date":"2020-11-08 14:00:00","value":23.0092},{"date":"2020-11-08 13:00:00","value":16.9139},{"date":"2020-11-08 12:00:00","value":23.3564},{"date":"2020-11-08 11:00:00","value":23.9546},{"date":"2020-11-08 10:00:00","value":24.3828},{"date":"2020-11-08 09:00:00","value":13.4063},{"date":"2020-11-08 08:00:00","value":17.5069},{"date":"2020-11-08 07:00:00","value":19.454},{"date":"2020-11-08 06:00:00","value":21.0313},{"date":"2020-11-08 05:00:00","value":23.8683},{"date":"2020-11-08 04:00:00","value":21.5449},{"date":"2020-11-08 03:00:00","value":24.8565},{"date":"2020-11-08 02:00:00","value":28.2466},{"date":"2020-11-08 01:00:00","value":36.9626},{"date":"2020-11-08 00:00:00","value":45.1248},{"date":"2020-11-07 23:00:00","value":56.5799},{"date":"2020-11-07 22:00:00","value":58.0402},{"date":"2020-11-07 21:00:00","value":57.1552},{"date":"2020-11-07 20:00:00","value":62.5976},{"date":"2020-11-07 19:00:00","value":68.776},{"date":"2020-11-07 18:00:00","value":56.1484},{"date":"2020-11-07 17:00:00","value":42.728},{"date":"2020-11-07 16:00:00","value":30.1175},{"date":"2020-11-07 15:00:00","value":31.8529},{"date":"2020-11-07 14:00:00","value":28.0848},{"date":"2020-11-07 13:00:00","value":23.7259},{"date":"2020-11-07 12:00:00","value":22.5223},{"date":"2020-11-07 11:00:00","value":35.7695},{"date":"2020-11-07 10:00:00","value":56.917},{"date":"2020-11-07 09:00:00","value":34.2559},{"date":"2020-11-07 08:00:00","value":18.1102},{"date":"2020-11-07 07:00:00","value":21.5267},{"date":"2020-11-07 06:00:00","value":12.5356},{"date":"2020-11-07 05:00:00","value":12.1152},{"date":"2020-11-07 04:00:00","value":15.9664},{"date":"2020-11-07 03:00:00","value":17.1926},{"date":"2020-11-07 02:00:00","value":15.9818},{"date":"2020-11-07 01:00:00","value":31.0049}]}

    Possible errors:
    >0 - OK
    -1 - Connection error
    -2 - HTT Code != 200
    -3 - Unable to parse json response
*/