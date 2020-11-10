/*
  GIOSRestApi.h - Library for getting sensor values from GIOS REST API. See details here: http://powietrze.gios.gov.pl/pjp/content/api
  Author - Tomasz Babiuk
*/
#ifndef GIOSRestApi_h
#define GIOSRestApi_h

#include "GIOSTimeParser.h"

enum gios_error {
    NoError,
    CannotConnect,
    Non200HttpCode,
    JsonParsingError
};

struct gios_result {
    double sensorData;
    gios_time time;
    gios_error error;
};

class GIOSRestApi
{
    public:
        static gios_result getSensorData(int sensorId);

    private:
};

#endif